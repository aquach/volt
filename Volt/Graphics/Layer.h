#pragma once

#pragma once

#include "Core/Core.h"

namespace Volt {

class Graphic;
class Renderer;

/**
 *  A layer containing many Graphics to be rendered to the screen.
 */
class Layer {
public:
    Layer (Renderer* renderer, int level)
        : m_renderer(renderer),
          m_level(level) { }

    ~Layer () { }

    void Render ();

    int Level () { return m_level; }

private:
    list<Graphic*> m_graphics;
    int m_level;
    Renderer* m_renderer;

    DISALLOW_COPY_AND_ASSIGN(Layer);
};

}
