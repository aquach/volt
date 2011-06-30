#pragma once

#include "Vector2.h"
#include <ostream>

using namespace std;

namespace Volt {

/** Describes a transformation in 2D space, via position
 *  (horizontal (x) position, vertical (y) position), rotation (an angle)
 *  and scale (horizontal (x) size, vertical (y) size).
 */
class Transform {
public:
    Transform ();
    Transform (const Transform& transform);

    /** Interpolate this Transform from prev and next, based on percent.
     *  t goes from 0.0 to 1.0.
     */
    void LerpTransform (Transform *prev, Transform *next, float percent);

    /** Retrieves a direction vector based on the rotation value. */
    Vector2 GetDirectionVector() const;

    Vector2 position;
    float rotation; // Degrees.
    Vector2 scale;

    Vector2 Apply (Vector2 v) const;
    Vector2 ApplyInverse (Vector2 v) const;

    void Load (const Json::Value& node);
    void Save (Json::Value& node) const;

    Vector2 xAxis () const { return Vector2(1, 0).Rotate(rotation); }
    Vector2 yAxis () const { return Vector2(0, 1).Rotate(rotation); }

    Transform Multiply (const Transform& other) const;
};

ostream& operator<< (ostream& stream, const Transform& other);

}
