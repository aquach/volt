#pragma once

#pragma once

#include "Core/Core.h"

namespace Volt {

class Layer

/**
 *  Main rendering engine.
 */
class Renderer {
public:
    Renderer () { }
    ~Renderer () { }

    void Render ();

private:
    list<Layer*> m_layers;

    DISALLOW_COPY_AND_ASSIGN(Renderer);
};

}
