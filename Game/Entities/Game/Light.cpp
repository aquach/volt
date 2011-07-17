#include "Game/Entities/Game/Light.h"
#include "Game/Editor/Property.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/GpuProgram.h"

REGISTER_ENTITY_(Light);

const int TEXTURE_WIDTH = 1024;
const int TEXTURE_HEIGHT = 1024;
const float LIGHT_LENGTH = 15;

GLuint MakeTexture () {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    return texture;
}

Light::Light ()
	: m_color(Volt::Color::white),
      m_intensity(1.0f) {
    AddTag("Light");
    CreatePhysicsBody();
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    GLuint shadowBuffer;
    glGenRenderbuffers(1, &shadowBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, shadowBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                          TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        shadowBuffer
    );

    /* Dummy color texture. */
    m_dummyTexture = MakeTexture();

    /* Parabolic mapping texture. */
    m_parabolicTexture = MakeTexture();

    /* Starting depth texture. */
    glGenTextures(1, &m_shadowTexture);
    glBindTexture(GL_TEXTURE_2D, m_shadowTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXTURE_WIDTH,
                 TEXTURE_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    m_distanceTexture = MakeTexture();

    glGenTextures(1, &m_lightTexture);
    glBindTexture(GL_TEXTURE_2D, m_lightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2 /* 2 pixel width */,
                 TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO: Rename.
    m_program = new Volt::GpuProgram;
    m_program->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_program->Attach(
        Volt::G_AssetManager->GetShader(
            "distance.frag", Volt::ShaderAsset::SHADER_FRAGMENT));

    m_program2 = new Volt::GpuProgram;
    m_program2->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_program2->Attach(
        Volt::G_AssetManager->GetShader(
            "parabolic.frag", Volt::ShaderAsset::SHADER_FRAGMENT));

    m_program3 = new Volt::GpuProgram;
    m_program3->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_program3->Attach(
        Volt::G_AssetManager->GetShader(
            "reduce.frag", Volt::ShaderAsset::SHADER_FRAGMENT));

    m_program4 = new Volt::GpuProgram;
    m_program4->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_program4->Attach(
        Volt::G_AssetManager->GetShader(
            "light.frag", Volt::ShaderAsset::SHADER_FRAGMENT));
}

Light::~Light () {
    delete m_program;
    delete m_program2;
    delete m_program3;
    delete m_program4;
}

void Light::Update () {
    return;
}

void CheckFramebufferStatus () {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    CHECK_EQ(status, GL_FRAMEBUFFER_COMPLETE)
        << "Frame buffer was not fully configured.";
}

void Light::ReloadShader () {
    m_program->Reload();
    m_program2->Reload();
}

void RenderPass () {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    Graphics::SetColor(Volt::Color::white);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex2i(0, 0);
    glTexCoord2i(1, 0);
    glVertex2i(1, 0);
    glTexCoord2i(1, 1);
    glVertex2i(1, 1);
    glTexCoord2i(0, 1);
    glVertex2i(0, 1);
    glEnd();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void Light::Render () {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);

    // Rerender entities around light to build shadow map.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_dummyTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, m_shadowTexture, 0);
    CheckFramebufferStatus();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    Graphics::Translate(Vector2(0.5, 0.5));
    glScalef(0.5f / LIGHT_LENGTH, 0.5f / LIGHT_LENGTH, 1);
    Graphics::Translate(-position());

    vector<Volt::Entity*> entities;
    Vector2 field = Vector2(LIGHT_LENGTH, LIGHT_LENGTH);
    scene()->GetEntitiesInArea (position() - field,
                                position() + field,
                                &entities);
    for (uint i = 0; i < entities.size(); i++) {
        if (dynamic_cast<Light*>(entities[i]))
            continue;
        entities[i]->Render();
    }
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);


    // Render distance map.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_distanceTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, 0, 0);
    CheckFramebufferStatus();

    Graphics::BindShader(m_program);
    glBindTexture(GL_TEXTURE_2D, m_shadowTexture);
    Graphics::SetShaderValue("shadowMap", 0);
    RenderPass();

    // Render parabolic map.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_parabolicTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, 0, 0);
    CheckFramebufferStatus();

    Graphics::BindShader(m_program2);
    glBindTexture(GL_TEXTURE_2D, m_distanceTexture);
    Graphics::SetShaderValue("distanceMap", 0);
    RenderPass();

    // Render final light map.
    glViewport(0, 0, 2, TEXTURE_HEIGHT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_lightTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, 0, 0);
    CheckFramebufferStatus();

    Graphics::BindShader(m_program3);
    glBindTexture(GL_TEXTURE_2D, m_parabolicTexture);
    Graphics::SetShaderValue("parabolicMap", 0);
    GLint loc = glGetUniformLocation(m_program3->handle(), "pixelSize");
    glUniform2f(loc, 1.0f / TEXTURE_WIDTH, 1.0f / TEXTURE_HEIGHT);
    RenderPass();

    glPopAttrib();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Render light!!
    glPushMatrix();
    Graphics::BindShader(m_program4);
    glBindTexture(GL_TEXTURE_2D, m_lightTexture);
    Graphics::SetShaderValue("lightMap", 0);
    Graphics::Translate(position());
    Graphics::RenderQuad(LIGHT_LENGTH * 2, LIGHT_LENGTH * 2);

    Graphics::BindShader(NULL);
    glPopMatrix();

    // TODO: Use RG + BA to double float precision.

    /*
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, m_distanceTexture);
    Graphics::RenderQuad(5, 5);

    Graphics::Translate(Vector2(5, 0));
    glBindTexture(GL_TEXTURE_2D, m_parabolicTexture);
    Graphics::RenderQuad(5, 5);

    Graphics::Translate(Vector2(5, 0));
    glBindTexture(GL_TEXTURE_2D, m_lightTexture);
    Graphics::RenderQuad(5, 5);
    glPopMatrix();
    */
}

void Light::CreatePhysicsBody () {
    DestroyBody();
    b2BodyDef def;
    def.type = b2_staticBody;
    m_body = CreateBody(def);

    b2CircleShape shape;
    shape.m_radius = 0.5;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;
    m_body->CreateFixture(&fixtureDef);
}

void Light::Load (const Json::Value& node) {
    CHECK(node["type"].asString() == "Light");
    CHECK(node.isMember("transform"));
    CHECK(node.isMember("color"));
    CHECK(node.isMember("intensity"));

    m_transform.Load(node["transform"]);
    m_color.Load(node["color"]);
    m_intensity = node["intensity"].asDouble();
    CreatePhysicsBody();
}

void Light::Save (Json::Value& node) const {
    node["type"] = "Light";
    m_transform.Save(node["transform"]);
    m_color.Save(node["color"]);
    node["intensity"] = m_intensity;
}

void Light::OnScaleChanged () {
    CreatePhysicsBody();
}

Light* Light::Clone () const {
    Light* newLight = new Light;
    newLight->CopyFrom(this);
    return newLight;
}

void Light::CopyFrom (const Light* other) {
    Entity::CopyFrom(other);
    m_intensity = other->m_intensity;
    m_color = other->m_color;
    CreatePhysicsBody();
}

void Light::GetProperties (vector<Property*>* properties) {
    Entity::GetProperties(properties);
    properties->push_back(new ColorProperty("Color", &m_color));
    properties->push_back(new FloatProperty("Intensity", &m_intensity));
}


