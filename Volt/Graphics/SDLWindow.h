#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Graphics/Input.h"
#include "Volt/Graphics/Viewport.h"

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

    virtual void SwapBuffers ();
    
    void Close ();

    void Screenshot (const string& filename);

    bool IsKeyPressed (SDLKey key);
    void UpdateInput ();

    void SetTitle (string title);

private:
    Game* m_game;
    void* m_screen;
};

#define G_Window dynamic_cast<Volt::SDLWindow*>(Volt::Viewport::Instance())
}
