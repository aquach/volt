#pragma once

#pragma once

#include "Core/Core.h"

namespace Volt {

class Layer;

/**
 *  Main rendering engine.
 */
class Renderer {
public:
    Renderer (int w, int h);
    ~Renderer () { }

    void Render ();

private:
    int m_width;
    int m_height;
    list<Layer*> m_layers;

    DISALLOW_COPY_AND_ASSIGN(Renderer);
};

}
