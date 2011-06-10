#pragma once

#include "Core/Core.h"

namespace Volt {

class SDL_Surface;

/**
 *  Manages the graphics window.
 */
class Window {
public:
    Window (const string& name, int w, int h, bool fullscreen);
    ~Window () { }

    void Close ();

private:
    SDL_Surface* m_screen;
    DISALLOW_COPY_AND_ASSIGN(Window);
};

}
