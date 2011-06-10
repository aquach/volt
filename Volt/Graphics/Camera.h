#pragma once

#include "Core/Core.h"

namespace Volt {

/**
 *  A camera in 2D space.
 */
class Camera {
public:
    Camera () { }
    ~Camera () { }

    float* GetViewTransform ();
private:
    C2Vector position;
    float scale;
};

}
