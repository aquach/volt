#include "LightManager.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Game/Entities/Game/Light.h"

const int TEXTURE_WIDTH = 512;
const int TEXTURE_HEIGHT = 512;

LightManager* LightManager::instance = NULL;

// TODO: Lights add their light together, but how does this interact
// with lighting the background?

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

LightManager::LightManager ()
    : m_scene(NULL),
      m_fbo(-1),
      m_dummyTexture(-1),
      m_depthTexture(-1),
      m_distanceTexture(-1),
      m_parabolicTexture(-1),
      m_shadowTexture(-1),
      m_lightTexture(-1),
      m_shadowShader(NULL),
      m_parabolicShader(NULL),
      m_reduceShader(NULL),
      m_lightShader(NULL),
      m_blurShader(NULL),
      m_debugDraw(false) {
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
    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXTURE_WIDTH,
                 TEXTURE_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    m_distanceTexture = MakeTexture();

    // Shadow texture is 2 x TEXTURE_HEIGHT.
    glGenTextures(1, &m_shadowTexture);
    glBindTexture(GL_TEXTURE_2D, m_shadowTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 2 /* 2 pixel width */,
                 TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Light texture map.
    glGenTextures(1, &m_lightTexture);
    glBindTexture(GL_TEXTURE_2D, m_lightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH,
                 TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
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

    m_blurShader = new Volt::GpuProgram;
    m_blurShader->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_blurShader->Attach(
        Volt::G_AssetManager->GetShader(
            "blur.frag", Volt::ShaderAsset::SHADER_FRAGMENT));
}

LightManager::~LightManager () {
    delete m_lightShader;
    delete m_reduceShader;
    delete m_parabolicShader;
    delete m_shadowShader;
    delete m_blurShader;
    GLuint textures[] = { m_dummyTexture, m_depthTexture, m_distanceTexture,
                          m_parabolicTexture, m_shadowTexture,
                          m_lightTexture };
    glDeleteTextures(sizeof(textures) / sizeof(GLuint), textures);
    glDeleteFramebuffers(1, &m_fbo);
}

void LightManager::RenderLight (Light* light) {
    LOG(INFO) << "START";
    float lightLength = light->maxDistance();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);

    // Rerender entities around light to build shadow map.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_dummyTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, m_depthTexture, 0);
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
    glScalef(0.5f / lightLength, 0.5f / lightLength, 1);
    Graphics::Translate(-light->position());

    LOG(INFO) << "PRE RENDER";

    for (uint i = 0; i < light->m_nearbyEntities.size(); i++) {
        if (dynamic_cast<Light*>(light->m_nearbyEntities[i]))
            continue;
        light->m_nearbyEntities[i]->Render();
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    LOG(INFO) << "PRE DISTANCE MAP";

    // Render distance map.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_distanceTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, 0, 0);
    CheckFramebufferStatus();

    Graphics::BindShader(m_shadowShader);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    Graphics::SetShaderValue("depthMap", 0);
    RenderPass();

    LOG(INFO) << "PRE PARA MAP";

    // Render parabolic map.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_parabolicTexture, 0);
    CheckFramebufferStatus();

    Graphics::BindShader(m_parabolicShader);
    glBindTexture(GL_TEXTURE_2D, m_distanceTexture);
    Graphics::SetShaderValue("distanceMap", 0);
    RenderPass();

    LOG(INFO) << "PRE SHADOW MAP";

    // Render shadow map.
    glViewport(0, 0, 2, TEXTURE_HEIGHT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_shadowTexture, 0);
    CheckFramebufferStatus();

    Graphics::BindShader(m_reduceShader);
    glBindTexture(GL_TEXTURE_2D, m_parabolicTexture);
    Graphics::SetShaderValue("parabolicMap", 0);
    Vector2 pixelSize(1.0f / TEXTURE_WIDTH, 1.0f / TEXTURE_HEIGHT);
    Graphics::SetShaderValue("pixelSize", pixelSize);
    RenderPass();

    LOG(INFO) << "PRE FINAL";

    // Render final image.
    glPopAttrib();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glPushMatrix();
    Graphics::SetBlend(Graphics::BLEND_ADDITIVE);
    Graphics::BindShader(m_lightShader);
    glBindTexture(GL_TEXTURE_2D, m_shadowTexture);
    Graphics::SetShaderValue("shadowMap", 0);
    Graphics::SetShaderValue("color", light->color());
    Graphics::SetShaderValue("coneAngle", light->coneAngle());
    Graphics::SetShaderValue("lightDir", light->transform().yAxis());
    Graphics::Translate(light->position());
    Graphics::RenderQuad(lightLength * 2, lightLength * 2);

    Graphics::BindShader(NULL);
    glPopMatrix();

    Graphics::SetBlend(Graphics::BLEND_NONE);

    if (m_debugDraw) {
        glPushMatrix();
        Graphics::Translate(light->position() + Vector2(1, 0));
        glBindTexture(GL_TEXTURE_2D, m_distanceTexture);
        Graphics::RenderQuad(2, 2);

        Graphics::Translate(Vector2(2, 0));
        glBindTexture(GL_TEXTURE_2D, m_parabolicTexture);
        Graphics::RenderQuad(2, 2);

        Graphics::Translate(Vector2(2, 0));
        glBindTexture(GL_TEXTURE_2D, m_shadowTexture);
        Graphics::RenderQuad(2, 2);

        glPopMatrix();
    }

    Graphics::BindTexture(NULL);
    LOG(INFO) << "END";
}
