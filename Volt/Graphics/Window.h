#pragma once

#include "Core/Core.h"
#include "Graphics/Input.h"

namespace Volt {

class Game;

/**
 *  Manages the graphics window.
 */
class Window {
public:
    Window (Game* game, const string& name,
            int w, int h, bool fullscreen);
    ~Window ();

    void Close ();

    void Screenshot (const string& filename);

    int width () const;
    int height () const;

    void UpdateInput ();

private:
    Game* m_game;
    void* m_screen;
    DISALLOW_COPY_AND_ASSIGN(Window);
};

}
