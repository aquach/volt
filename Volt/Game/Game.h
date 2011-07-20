#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Graphics/Input.h"

namespace Volt {

class AppTime;
class AssetManager;
class DataSource;
class Graphics;
class KeyBindings;
class PhysicsManager;
class Scene;
class SoundManager;
class SDLWindow;

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
    
    float fps () const { return 1 / m_ticksPerFrame * 1000; }

    Scene* currentScene () const { return m_currentScene; }
    void SetScene (Scene* scene);

    static void Register (Game* game) { instance = game; }
    static Game* Instance () { return instance; }

    void OnKeyEvent (SDL_KeyboardEvent event);
    void OnMouseButtonEvent (SDL_MouseButtonEvent event);
    void OnMouseMoveEvent (SDL_MouseMotionEvent event);

private:
    string m_name;
    bool m_willQuit;

    Scene* m_currentScene;
    Scene* m_switchToScene;

    AssetManager* m_assetManager;
    Graphics* m_graphics;
    KeyBindings* m_bindings;
    PhysicsManager* m_physicsManager;
    SoundManager* m_soundManager;
    AppTime* m_appTime;
    Time m_time;
    SDLWindow* m_window;

    long m_lastTick;
    float m_ticksPerFrame;

    static Game* instance;

    DISALLOW_COPY_AND_ASSIGN(Game);
};

}
