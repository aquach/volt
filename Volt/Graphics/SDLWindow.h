#pragma once

#include "Core/Core.h"
#include "Graphics/Input.h"
#include "Viewport.h"

namespace Volt {

class Game;

/**
 *  Manages the graphics window.
 */
class SDLWindow : public Viewport {
public:
    SDLWindow (Game* game, const string& name,
            int w, int h, bool fullscreen);
    ~SDLWindow ();

    virtual int width () const;
    virtual int height () const;
    
    void Close ();

    void Screenshot (const string& filename);

    bool IsKeyPressed (SDLKey key);
    void UpdateInput ();

private:
    Game* m_game;
    void* m_screen;
};

#define G_Window dynamic_cast<Volt::SDLWindow*>(Volt::Viewport::Instance())
}
