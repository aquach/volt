#pragma once

#include "Game/Core/Core.h"
#include "Game/Graphics/Graphics.h"

namespace Volt {
    class GpuProgram;
    class RenderSurface;
    class Scene;
}

class Light;

class LightManager {
public:
    LightManager ();
    ~LightManager ();

    static void Register (LightManager* manager) { instance = manager; }
    static LightManager* Instance () { return instance; }

    void RenderLight (Light* light);

    void SetDebugDraw (bool enabled ) { m_debugDraw = enabled; }

private:
    friend class GameScene;
    friend class EditorScene;
    Volt::Scene* m_scene;

    bool m_debugDraw;

    enum FBOType {
        FBO_DEPTH = 0,
        FBO_PARABOLIC,
        FBO_SHADOW,
        FBO_COUNT
    };

    enum TextureType {
        TEXTURE_DUMMY = 0,
        TEXTURE_DEPTH,
        TEXTURE_PARABOLIC,
        TEXTURE_SHADOW,
        TEXTURE_COUNT
    };

    enum ShaderType {
        SHADER_PARABOLIC = 0,
        SHADER_SHADOW,
        SHADER_LIGHT,
        SHADER_BLUR,
        SHADER_ATTENUATE,
        SHADER_COUNT
    };

    GLuint m_fbos[FBO_COUNT];
    GLuint m_textures[TEXTURE_COUNT];
    Volt::GpuProgram* m_shaders[SHADER_COUNT];
    static const int NUM_SAMPLES = 4;
    Volt::RenderSurface* m_bloomPass1[NUM_SAMPLES];
    Volt::RenderSurface* m_bloomPass2[NUM_SAMPLES];

    static LightManager* instance;

    DISALLOW_COPY_AND_ASSIGN(LightManager);
};

#define G_LightManager LightManager::Instance()
