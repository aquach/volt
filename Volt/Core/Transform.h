#pragma once

#include "Vector2.h"

namespace Volt {

/** Describes a transformation in 2D space, via position
 *  (horizontal (x) position, vertical (y) position), rotation (an angle)
 *  and scale (horizontal (x) size, vertical (y) size).
 */
class Transform {
public:
    Transform ();
    Transform (const Transform &transform);

    /** Interpolate this Transform from prev and next, based on percent.
     *  t goes from 0.0 to 1.0.
     */
    void LerpTransform (Transform *prev, Transform *next, float percent);

    /** Retrieves a direction vector based on the rotation value. */
    Vector2 GetDirectionVector() const;

    Vector2 position;
    float rotation; // Degrees.
    Vector2 scale;
};
}
