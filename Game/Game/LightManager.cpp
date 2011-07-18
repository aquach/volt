#include "LightManager.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Game/Entities/Game/Light.h"

const int TEXTURE_WIDTH = 1024;
const int TEXTURE_HEIGHT = 1024;
const float LIGHT_LENGTH = 15;

LightManager* LightManager::instance = NULL;

// Constructs a color texture.
GLuint MakeTexture () {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, TEXTURE_WIDTH, TEXTURE_HEIGHT,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    return texture;
}

void CheckFramebufferStatus () {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    CHECK_EQ(status, GL_FRAMEBUFFER_COMPLETE)
        << "Frame buffer was not fully configured.";
}

// Renders a quad across the entire screen, for each pass.
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

LightManager::LightManager () {
    CHECK(Graphics::initialized());

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // TODO: Need this?
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

    // Light texture is 2 x TEXTURE_HEIGHT.
    glGenTextures(1, &m_lightTexture);
    glBindTexture(GL_TEXTURE_2D, m_lightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 2 /* 2 pixel width */,
                 TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_shadowShader = new Volt::GpuProgram;
    m_shadowShader->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_shadowShader->Attach(
        Volt::G_AssetManager->GetShader(
            "distance.frag", Volt::ShaderAsset::SHADER_FRAGMENT));

    m_parabolicShader = new Volt::GpuProgram;
    m_parabolicShader->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_parabolicShader->Attach(
        Volt::G_AssetManager->GetShader(
            "parabolic.frag", Volt::ShaderAsset::SHADER_FRAGMENT));

    m_reduceShader = new Volt::GpuProgram;
    m_reduceShader->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_reduceShader->Attach(
        Volt::G_AssetManager->GetShader(
            "reduce.frag", Volt::ShaderAsset::SHADER_FRAGMENT));

    m_lightShader = new Volt::GpuProgram;
    m_lightShader->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_lightShader->Attach(
        Volt::G_AssetManager->GetShader(
            "light.frag", Volt::ShaderAsset::SHADER_FRAGMENT));
}

LightManager::~LightManager () {
    delete m_lightShader;
    delete m_reduceShader;
    delete m_parabolicShader;
    delete m_shadowShader;
    GLuint textures[5] = { m_dummyTexture, m_shadowTexture, m_distanceTexture,
                           m_parabolicTexture, m_lightTexture };
    glDeleteTextures(5, textures);
    glDeleteFramebuffers(1, &m_fbo);
}

void LightManager::RenderLight (Light* light) {
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
    Graphics::Translate(-light->position());

    vector<Volt::Entity*> entities;
    Vector2 field = Vector2(LIGHT_LENGTH, LIGHT_LENGTH);
    m_scene->GetEntitiesInArea (light->position() - field,
                                light->position() + field,
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

    Graphics::BindShader(m_shadowShader);
    glBindTexture(GL_TEXTURE_2D, m_shadowTexture);
    Graphics::SetShaderValue("shadowMap", 0);
    RenderPass();

    // Render parabolic map.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_parabolicTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, 0, 0);
    CheckFramebufferStatus();

    Graphics::BindShader(m_parabolicShader);
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

    Graphics::BindShader(m_reduceShader);
    glBindTexture(GL_TEXTURE_2D, m_parabolicTexture);
    Graphics::SetShaderValue("parabolicMap", 0);
    GLint loc = glGetUniformLocation(m_reduceShader->handle(), "pixelSize");
    glUniform2f(loc, 1.0f / TEXTURE_WIDTH, 1.0f / TEXTURE_HEIGHT);
    RenderPass();

    glPopAttrib();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Render light!!
    glPushMatrix();
    Graphics::BindShader(m_lightShader);
    glBindTexture(GL_TEXTURE_2D, m_lightTexture);
    Graphics::SetShaderValue("lightMap", 0);
    Graphics::Translate(light->position());
    Graphics::RenderQuad(LIGHT_LENGTH * 2, LIGHT_LENGTH * 2);

    Graphics::BindShader(NULL);
    glPopMatrix();

    // TODO: Use RG + BA to double float precision.

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

    /*glBindTexture(GL_TEXTURE_2D, m_distanceTexture);
    float* pixels = new float[TEXTURE_WIDTH * TEXTURE_HEIGHT];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, pixels);
    for (int i = 0; i < TEXTURE_WIDTH * TEXTURE_HEIGHT; i += 300)
        LOG(INFO) << pixels[i];
    CHECK(false);
    */
}
