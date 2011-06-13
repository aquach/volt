#pragma once

#include "Core/Core.h"

namespace Volt {

/**
 *  Manages the graphics window.
 */
class Window {
public:
    Window (const string& name, int w, int h, bool fullscreen);
    ~Window ();

    void Close ();

    void Screenshot (const string& filename);

private:
    void* m_screen;
    DISALLOW_COPY_AND_ASSIGN(Window);
};

}
