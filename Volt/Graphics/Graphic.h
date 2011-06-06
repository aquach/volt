#pragma once

#include "Core/Core.h"

namespace Volt {

class Renderer;

/**
 *  A thing, probably linked to an Entity, to be rendered.
 */
class Graphic {
public:
    Graphic () { }
    ~Graphic () { }

    virtual void Render () = 0;

private:
    DISALLOW_COPY_AND_ASSIGN(Graphic);
};

}
