#pragma once

#include "Volt/Core/Core.h"

namespace Volt {

/**
 *  Manages an area that draws OpenGL, maybe a window or a QT widget.
 */
class Viewport {
public:
    Viewport ();
    ~Viewport ();

    virtual int width () const = 0;
    virtual int height () const = 0;

    virtual void SwapBuffers () = 0;

    static void Register (Viewport* viewport) { instance = viewport; }
    static Viewport* Instance () { return instance; }

protected:
    static Viewport* instance;    

private:
    DISALLOW_COPY_AND_ASSIGN(Viewport);
};

#define G_Viewport Volt::Viewport::Instance()

}
