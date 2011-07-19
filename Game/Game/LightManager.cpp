#include "LightManager.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Game/Entities/Game/Light.h"

const int TEXTURE_WIDTH = 512;
const int TEXTURE_HEIGHT = 512;

LightManager* LightManager::instance = NULL;

typedef map<string, int> Times;
static int lightCount;
static Times times;

// TODO: Lights add their light together, but how does this interact
// with lighting the background?

void ConfigureTexture (GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, TEXTURE_WIDTH, TEXTURE_HEIGHT,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
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
      m_debugDraw(false) {
    CHECK(Graphics::initialized());

    glGenFramebuffers(FBO_COUNT, m_fbos);
    glGenTextures(TEXTURE_COUNT, m_textures);

    /* Dummy color texture. */
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_DUMMY]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TEXTURE_WIDTH,
                 TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    /* Starting depth texture. */
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_DEPTH]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXTURE_WIDTH,
                 TEXTURE_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    ConfigureTexture(m_textures[TEXTURE_DISTANCE]);

    /* Parabolic mapping texture. */
    ConfigureTexture(m_textures[TEXTURE_PARABOLIC]);

    // Shadow texture is 2 x TEXTURE_HEIGHT.
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_SHADOW]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 2 /* 2 pixel width */,
                 TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Bind textures to each frame buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_DEPTH]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_textures[TEXTURE_DUMMY], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, m_textures[TEXTURE_DEPTH], 0);
    CheckFramebufferStatus();
                           
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_DISTANCE]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_textures[TEXTURE_DISTANCE], 0);
    CheckFramebufferStatus();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_PARABOLIC]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_textures[TEXTURE_PARABOLIC], 0);
    CheckFramebufferStatus();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_SHADOW]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_textures[TEXTURE_SHADOW], 0);
    CheckFramebufferStatus();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    for (int i = 0; i < SHADER_COUNT; i++)
        m_shaders[i] = new Volt::GpuProgram;
        
    m_shaders[SHADER_DISTANCE]->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_shaders[SHADER_DISTANCE]->Attach(
        Volt::G_AssetManager->GetShader(
            "distance.frag", Volt::ShaderAsset::SHADER_FRAGMENT));

    m_shaders[SHADER_PARABOLIC]->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_shaders[SHADER_PARABOLIC]->Attach(
        Volt::G_AssetManager->GetShader(
            "parabolic.frag", Volt::ShaderAsset::SHADER_FRAGMENT));

    m_shaders[SHADER_SHADOW]->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_shaders[SHADER_SHADOW]->Attach(
        Volt::G_AssetManager->GetShader(
            "reduce.frag", Volt::ShaderAsset::SHADER_FRAGMENT));

    m_shaders[SHADER_LIGHT]->Attach(
        Volt::G_AssetManager->GetShader(
            "standard.vert", Volt::ShaderAsset::SHADER_VERTEX));
    m_shaders[SHADER_LIGHT]->Attach(
        Volt::G_AssetManager->GetShader(
            "light.frag", Volt::ShaderAsset::SHADER_FRAGMENT));
}

LightManager::~LightManager () {
    for (int i = 0; i < SHADER_COUNT; i++)
        delete m_shaders[i];
        
    glDeleteTextures(TEXTURE_COUNT, m_textures);
    glDeleteFramebuffers(FBO_COUNT, m_fbos);

    LOG(INFO) << "LIGHT PERFORMANCE";
    
    long total = 0;
    FOR_(Times::iterator, i, times) {
        total += i->second;
    }
    FOR_(Times::iterator, i, times) {
        LOG(INFO) << i->first << ": "
                  << i->second / lightCount << " microsecs/light "
                  << (int)((float)i->second / total * 100) << "%";
    }
}

void LightManager::RenderLight (Light* light) {
    lightCount++;
    long usecs;
    usecs = Volt::GetMicroseconds();
    float lightLength = light->maxDistance();

    // Rerender entities around light to build shadow map.
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_DEPTH]);
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);

    times["start-binding1"] += Volt::GetMicroseconds() - usecs;
    usecs = Volt::GetMicroseconds();    

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

    times["start-setup"] += Volt::GetMicroseconds() - usecs;
    usecs = Volt::GetMicroseconds();

    for (uint i = 0; i < light->m_nearbyEntities.size(); i++) {
        if (dynamic_cast<Light*>(light->m_nearbyEntities[i]))
            continue;
        light->m_nearbyEntities[i]->Render();
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    times["render"] += Volt::GetMicroseconds() - usecs;
    usecs = Volt::GetMicroseconds();

    // Render distance map.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_DISTANCE]);
    Graphics::BindShader(m_shaders[SHADER_DISTANCE]);
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_DEPTH]);
    Graphics::SetShaderValue("depthMap", 0);
    RenderPass();

    times["distance"] += Volt::GetMicroseconds() - usecs;
    usecs = Volt::GetMicroseconds();

    // Render parabolic map.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_PARABOLIC]);
    Graphics::BindShader(m_shaders[SHADER_PARABOLIC]);
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_DISTANCE]);
    Graphics::SetShaderValue("distanceMap", 0);
    RenderPass();

    times["parabolic"] += Volt::GetMicroseconds() - usecs;
    usecs = Volt::GetMicroseconds();

    // Render shadow map.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_SHADOW]);
    glViewport(0, 0, 2, TEXTURE_HEIGHT);
    CheckFramebufferStatus();

    Graphics::BindShader(m_shaders[SHADER_SHADOW]);
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_PARABOLIC]);
    Graphics::SetShaderValue("parabolicMap", 0);
    Vector2 pixelSize(1.0f / TEXTURE_WIDTH, 1.0f / TEXTURE_HEIGHT);
    Graphics::SetShaderValue("pixelSize", pixelSize);
    RenderPass();

    times["shadow"] += Volt::GetMicroseconds() - usecs;
    usecs = Volt::GetMicroseconds();

    // Render final image.
    glPopAttrib();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glPushMatrix();
    Graphics::SetBlend(Graphics::BLEND_ADDITIVE);
    Graphics::BindShader(m_shaders[SHADER_LIGHT]);
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_SHADOW]);
    Graphics::SetShaderValue("shadowMap", 0);
    Graphics::SetShaderValue("color", light->color());
    Graphics::SetShaderValue("coneAngle", light->coneAngle());
    Graphics::SetShaderValue("lightDir", light->transform().yAxis());
    Graphics::Translate(light->position());
    Graphics::RenderQuad(lightLength * 2, lightLength * 2);

    times["finalrender"] += Volt::GetMicroseconds() - usecs;
    usecs = Volt::GetMicroseconds();
    
    Graphics::BindShader(NULL);
    glPopMatrix();

    Graphics::SetBlend(Graphics::BLEND_NONE);

    if (m_debugDraw) {
        glPushMatrix();
        Graphics::Translate(light->position() + Vector2(1, 0));
        glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_DISTANCE]);
        Graphics::RenderQuad(2, 2);

        Graphics::Translate(Vector2(2, 0));
        glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_PARABOLIC]);
        Graphics::RenderQuad(2, 2);

        Graphics::Translate(Vector2(2, 0));
        glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_SHADOW]);
        Graphics::RenderQuad(2, 2);

        glPopMatrix();
    }

    Graphics::BindTexture(NULL);
    times["end"] += Volt::GetMicroseconds() - usecs;
}
