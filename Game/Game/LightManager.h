#pragma once

#include "Game/Core/Core.h"
#include "Game/Graphics/Graphics.h"

namespace Volt {
    class GpuProgram;
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

    GLuint m_fbo;
    GLuint m_dummyTexture;
    GLuint m_depthTexture;
    GLuint m_distanceTexture;
    GLuint m_parabolicTexture;
    GLuint m_shadowTexture;
    GLuint m_lightTexture;
    Volt::GpuProgram* m_shadowShader;
    Volt::GpuProgram* m_parabolicShader;
    Volt::GpuProgram* m_reduceShader;
    Volt::GpuProgram* m_lightShader;
    Volt::GpuProgram* m_blurShader;

    static LightManager* instance;

    DISALLOW_COPY_AND_ASSIGN(LightManager);
};

#define G_LightManager LightManager::Instance()
