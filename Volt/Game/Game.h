#pragma once

#include "Core/Core.h"

namespace Volt {

class AssetManager;
class DataSource;
class Graphics;
class Scene;
class Window;

/**
 *  Manages the entire game.
 */
class Game {
public:
    Game (const string& name, const DataSource* source, int w = 1024,
          int h = 768, bool fullscreen = false);
    ~Game ();

    void Run ();
    void Quit ();

    float dt () const { return m_dt; }
    float fps () const { return 1 / m_ticksPerFrame * 1000; }

    Scene* currentScene () const { return m_currentScene; }
    void SetScene (Scene* scene);

    void UpdateInput ();

    AssetManager* assetManager() { return m_assetManager; } // TODO: HACK

private:
    string m_name;
    bool m_willQuit;

    Scene* m_currentScene;
    Scene* m_switchToScene;

    Graphics* m_graphics;
    Window* m_window;
    AssetManager* m_assetManager;
    Time m_time;

    long m_lastTick;
    float m_ticksPerFrame;
    float m_dt;

    DISALLOW_COPY_AND_ASSIGN(Game);
};

}
