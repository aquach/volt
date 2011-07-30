#include "LightManager.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Volt/Graphics/RenderSurface.h"
#include "Game/Entities/Game/Light.h"

const int TEXTURE_WIDTH = 512;
const int TEXTURE_HEIGHT = 512;
const int LIGHT_TEXTURE_WIDTH = 1024;
const int LIGHT_TEXTURE_HEIGHT = 1024;

LightManager* LightManager::instance = NULL;

typedef map<string, int> Times;
static int lightCount;
static Times times;

// TODO: Lights add their light together, but how does this interact
// with lighting the background?

// First render of light seems to take a really long time. Might be
// GPU warming up or something.

void ConfigureTexture (GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

LightManager::LightManager () :
    m_scene(NULL), m_debugDraw(false) {
    CHECK(Graphics::initialized());

    glGenFramebuffers(FBO_COUNT, m_fbos);
    glGenTextures(TEXTURE_COUNT, m_textures);

    /* Dummy color texture. */
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_DUMMY]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    /* Starting depth texture. */
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_DEPTH]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXTURE_WIDTH,
                 TEXTURE_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

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

    int width = LIGHT_TEXTURE_WIDTH;
    int height = LIGHT_TEXTURE_HEIGHT;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        CHECK_GE(width, 0);
        CHECK_GE(height, 0);
        LOG(INFO) << "Creating " << width << " x " << height << " sample...";
        m_bloomPass1[i] = new Volt::RenderSurface(width, height, false, false,
                                                  true);
        m_bloomPass2[i] = new Volt::RenderSurface(width, height, false, false,
                                                  true);
        width /= 2;
        height /= 2;
    }

    m_finalSurface = new Volt::RenderSurface(LIGHT_TEXTURE_WIDTH,
                                             LIGHT_TEXTURE_HEIGHT, false,
                                             false, true);

    // Bind textures to each frame buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_DEPTH]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_textures[TEXTURE_DUMMY], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           m_textures[TEXTURE_DEPTH], 0);
    Volt::RenderSurface::CheckStatus();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_PARABOLIC]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_textures[TEXTURE_PARABOLIC], 0);
    Volt::RenderSurface::CheckStatus();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_SHADOW]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_textures[TEXTURE_SHADOW], 0);
    Volt::RenderSurface::CheckStatus();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    for (int i = 0; i < SHADER_COUNT; i++)
        m_shaders[i] = new Volt::GpuProgram;

    m_shaders[SHADER_PARABOLIC]->Attach(
        G_AssetManager->GetShader("Shaders/standard.vert",
                                        Volt::ShaderAsset::SHADER_VERTEX));
    m_shaders[SHADER_PARABOLIC]->Attach(
        G_AssetManager->GetShader("Shaders/parabolic.frag",
                                        Volt::ShaderAsset::SHADER_FRAGMENT));

    m_shaders[SHADER_SHADOW]->Attach(
        G_AssetManager->GetShader("Shaders/standard.vert",
                                        Volt::ShaderAsset::SHADER_VERTEX));
    m_shaders[SHADER_SHADOW]->Attach(
        G_AssetManager->GetShader("Shaders/reduce.frag",
                                        Volt::ShaderAsset::SHADER_FRAGMENT));

    m_shaders[SHADER_LIGHT]->Attach(
        G_AssetManager->GetShader("Shaders/standard.vert",
                                        Volt::ShaderAsset::SHADER_VERTEX));
    m_shaders[SHADER_LIGHT]->Attach(
        G_AssetManager->GetShader("Shaders/light.frag",
                                        Volt::ShaderAsset::SHADER_FRAGMENT));

    m_shaders[SHADER_BLUR]->Attach(
        G_AssetManager->GetShader("Shaders/standard.vert",
                                        Volt::ShaderAsset::SHADER_VERTEX));
    m_shaders[SHADER_BLUR]->Attach(
        G_AssetManager->GetShader("Shaders/blur.frag",
                                        Volt::ShaderAsset::SHADER_FRAGMENT));

    m_shaders[SHADER_ATTENUATE]->Attach(
        G_AssetManager->GetShader("Shaders/standard.vert",
                                        Volt::ShaderAsset::SHADER_VERTEX));
    m_shaders[SHADER_ATTENUATE]->Attach(
        G_AssetManager->GetShader("Shaders/attenuate.frag",
                                        Volt::ShaderAsset::SHADER_FRAGMENT));
}

LightManager::~LightManager () {
    for (int i = 0; i < SHADER_COUNT; i++)
        delete m_shaders[i];

    for (int i = 0; i < NUM_SAMPLES; i++) {
        delete m_bloomPass1[i];
        delete m_bloomPass2[i];
    }

    glDeleteTextures(TEXTURE_COUNT, m_textures);
    glDeleteFramebuffers(FBO_COUNT, m_fbos);

    long total = 0;
    FOR_(Times::iterator, i, times) {
        total += i->second;
    }

    // Breakdown of time spent in each part of light rendering. Each pass
    // is the average of the time it took for all lights over that pass.
    LOG(PERF) << "== LIGHT PERFORMANCE (" << lightCount << " lights) ==";
    FOR_(Times::iterator, i, times) {
        LOG(PERF) << i->first << ": avg " << i->second / lightCount
            << " usecs " << (int)((float)i->second / total * 100) << "%";
    }

    if (lightCount > 0)
        LOG(PERF) << "TOTAL: " << total / lightCount << " usecs";
}

void LightManager::RenderCachedStaticLight (Light* light) {
    Graphics::SetBlend(Graphics::BLEND_ADDITIVE);
    glPushMatrix();

    Graphics::SetColor(Volt::Color::white);
    Graphics::Translate(light->position());
    Graphics::BindTexture(light->m_staticMap);
    float lightLength = light->maxDistance();
    Graphics::RenderQuad(lightLength * 2, lightLength * 2);
    Graphics::BindTexture(NULL);

    glPopMatrix();
    Graphics::SetBlend(Graphics::BLEND_NONE);
}

void LightManager::RenderLight (Light* light) {
    // Viewport clipping.
    if (!m_scene->camera()->worldBounds().Intersects(light->renderBounds()))
        return;

    if (light->m_static && light->m_staticMap.HasAsset()) {
        RenderCachedStaticLight(light);
        return;
    }

    GLint oldFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFrameBuffer);

    lightCount++;
    long usecs, elapsed;
    usecs = Volt::GetMicroseconds();

    float lightLength = light->maxDistance();

    // Rerender entities around light to build shadow map.

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_DEPTH]);
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);

    elapsed = Volt::GetMicroseconds() - usecs;
    times["1-start-binding1"] += elapsed;
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

    elapsed = Volt::GetMicroseconds() - usecs;
    times["2-start-setup"] += elapsed;
    usecs = Volt::GetMicroseconds();

    for (uint i = 0; i < light->m_nearbyEntities.size(); i++) {
        if (dynamic_cast<Light*> (light->m_nearbyEntities[i]))
            continue;
        if (Entity* e = dynamic_cast<Entity*>(light->m_nearbyEntities[i])) {
            if (!e->occludesLight())
                continue;
        }
        light->m_nearbyEntities[i]->Render();
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    elapsed = Volt::GetMicroseconds() - usecs;
    times["3-render"] += elapsed;
    usecs = Volt::GetMicroseconds();

    // Render distance map.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_PARABOLIC]);
    Graphics::BindShader(m_shaders[SHADER_PARABOLIC]);
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_DEPTH]);
    Graphics::SetShaderValue("depthMap", 0);
    Volt::RenderSurface::RenderPass();

    elapsed = Volt::GetMicroseconds() - usecs;
    times["4-parabolic"] += elapsed;
    usecs = Volt::GetMicroseconds();

    // Render shadow map.
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[FBO_SHADOW]);
    glViewport(0, 0, 2, TEXTURE_HEIGHT);

    Graphics::BindShader(m_shaders[SHADER_SHADOW]);
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_PARABOLIC]);
    Graphics::SetShaderValue("parabolicMap", 0);
    Vector2 pixelSize(1.0f / TEXTURE_WIDTH, 1.0f / TEXTURE_HEIGHT);
    Graphics::SetShaderValue("pixelSize", pixelSize);
    Volt::RenderSurface::RenderPass();

    elapsed = Volt::GetMicroseconds() - usecs;
    times["5-shadow"] += elapsed;
    usecs = Volt::GetMicroseconds();

    glViewport(0, 0, LIGHT_TEXTURE_WIDTH, LIGHT_TEXTURE_HEIGHT);

    // Render light.
    m_bloomPass2[0]->Bind();
    Graphics::BindShader(m_shaders[SHADER_LIGHT]);
    glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_SHADOW]);
    Graphics::SetShaderValue("shadowMap", 0);
    Graphics::SetShaderValue("color", light->color());
    Graphics::SetShaderValue("coneAngle", light->coneAngle());
    Graphics::SetShaderValue("lightDir", light->transform().yAxis());
    Volt::RenderSurface::RenderPass();

    elapsed = Volt::GetMicroseconds() - usecs;
    times["6-light"] += elapsed;
    usecs = Volt::GetMicroseconds();

    /* Render downsampled and blurred horizontally from bloomPass2[0] into
     * bloomPass1. */
    Graphics::BindShader(m_shaders[SHADER_BLUR]);
    glBindTexture(GL_TEXTURE_2D, m_bloomPass2[0]->texture());
    for (int i = 0; i < NUM_SAMPLES; i++) {
        glViewport(0, 0, m_bloomPass1[i]->width(), m_bloomPass1[i]->height());
        Graphics::SetShaderValue("offset", Vector2(1.2f
            / m_bloomPass1[i]->width(), 0));
        m_bloomPass1[i]->Bind();
        Volt::RenderSurface::RenderPass();
    }

    elapsed = Volt::GetMicroseconds() - usecs;
    times["7-bloom1"] += elapsed;
    usecs = Volt::GetMicroseconds();

    /* Render blurred vertically from bloomPass1 into bloomPass2. */
    for (int i = 0; i < NUM_SAMPLES; i++) {
        glViewport(0, 0, m_bloomPass2[i]->width(), m_bloomPass2[i]->height());
        Graphics::SetShaderValue("offset", Vector2(0, 1.2f
            / m_bloomPass2[i]->height()));
        m_bloomPass2[i]->Bind();
        glBindTexture(GL_TEXTURE_2D, m_bloomPass1[i]->texture());
        Volt::RenderSurface::RenderPass();
    }

    elapsed = Volt::GetMicroseconds() - usecs;
    times["8-bloom2"] += elapsed;
    usecs = Volt::GetMicroseconds();

    // Render final image.
    m_finalSurface->Bind();
    glViewport(0, 0, m_finalSurface->width(), m_finalSurface->height());
    Graphics::Clear();
    Graphics::SetBlend(Graphics::BLEND_ADDITIVE);
    Graphics::BindShader(m_shaders[SHADER_ATTENUATE]);

    for (int i = 0; i < NUM_SAMPLES; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        char buffer[8];
        sprintf(buffer, "pass%d", i);
        Graphics::SetShaderValue(buffer, i);
        glBindTexture(GL_TEXTURE_2D, m_bloomPass2[i]->texture());
    }
    glActiveTexture(GL_TEXTURE0);
    Volt::RenderSurface::RenderPass();
    Graphics::BindShader(NULL);

    glPopAttrib();

    if (light->m_static && !light->m_staticMap.HasAsset()) {
        // Save static map.
        char path[128];
        sprintf(path, "%s/%s.bmp", Light::STATIC_MAP_FOLDER, light->m_name.c_str());

        char filename[128];
        sprintf(filename, "%s/%s", G_AssetManager->sourcePath().c_str(), path);

        Graphics::SaveTextureToFile(m_finalSurface->texture(), filename);
        LOG(INFO) << "Saving static light map to " << path << "...";
        light->m_staticMap = G_AssetManager->GetTexture(path);
        light->m_staticMap->Reload();
    }

    glPushMatrix();
    Graphics::Translate(light->position());
    glBindFramebuffer(GL_FRAMEBUFFER, oldFrameBuffer);
    glBindTexture(GL_TEXTURE_2D, m_finalSurface->texture());
    Graphics::RenderQuad(lightLength * 2, lightLength * 2);
    glPopMatrix();

    elapsed = Volt::GetMicroseconds() - usecs;
    times["9-finalrender"] += elapsed;
    usecs = Volt::GetMicroseconds();

    Graphics::SetBlend(Graphics::BLEND_NONE);

    if (m_debugDraw) {
        glPushMatrix();
        Graphics::Translate(light->position() + Vector2(1, 0));
        glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_DEPTH]);
        Graphics::RenderQuad(2, 2);

        Graphics::Translate(Vector2(2, 0));
        glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_PARABOLIC]);
        Graphics::RenderQuad(2, 2);

        Graphics::Translate(Vector2(2, 0));
        glBindTexture(GL_TEXTURE_2D, m_textures[TEXTURE_SHADOW]);
        Graphics::RenderQuad(2, 2);

        Graphics::Translate(Vector2(-4, 2));
        for (int i = 0; i < NUM_SAMPLES; i++) {
            Graphics::Translate(Vector2(2, 0));
            glBindTexture(GL_TEXTURE_2D, m_bloomPass1[i]->texture());
            Graphics::RenderQuad(2, 2);
        }

        for (int i = 0; i < NUM_SAMPLES; i++) {
            Graphics::Translate(Vector2(2, 0));
            glBindTexture(GL_TEXTURE_2D, m_bloomPass2[i]->texture());
            Graphics::RenderQuad(2, 2);
        }

        Graphics::Translate(Vector2(2, 0));
        glBindTexture(GL_TEXTURE_2D, m_finalSurface->texture());
        Graphics::RenderQuad(2, 2);

        glPopMatrix();
    }

    Graphics::BindTexture(NULL);
    elapsed = Volt::GetMicroseconds() - usecs;
    times["10-end"] += elapsed;
}
