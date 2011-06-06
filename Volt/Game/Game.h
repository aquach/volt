#pragma once

#include "Core/Core.h"

namespace Volt {

class AssetManager;
class Renderer;
class Scene;

/**
 *  Manages the entire game.
 */
class Game {
public:
    Game (const string& name, int w = 1024, int h = 768,
          bool fullscreen = false);
    ~Game () { }

    void Run ();
    void Quit ();

    float dt () const { return m_dt; }
    float fps () const { return 1 / m_ticksPerFrame * 1000; }

    Scene* GetScene () const;
    //void SetScene (Scene* scene);

private:
    string m_name;
    bool m_willQuit;

    list<Scene*> m_scenes;

    AssetManager* m_assetManager;
    Renderer* m_renderer;
    Time m_time;

    long m_lastTick;
    float m_ticksPerFrame;
    float m_dt;

    DISALLOW_COPY_AND_ASSIGN(Game);
};

}
