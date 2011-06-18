#pragma once

#include "Vector2.h"
#include "Math.h"
#include "Macros.h"

namespace Volt {

class BBox {
public:
    Vector2 min, max;

    BBox ()
        : min(Vector2(-INFINITY, -INFINITY)),
          max(Vector2(INFINITY, INFINITY)) { }

    BBox (const Vector2& v1, const Vector2& v2) {
        min.Set(MIN(v1.x, v2.x), MIN(v1.y, v2.y));
        max.Set(MAX(v1.x, v2.x), MAX(v1.y, v2.y));
    }

    BBox (const BBox& other)
        : min(other.min),
          max(other.max) {
    }

    BBox Union (const BBox& other) const;
    BBox Union (const Vector2& point) const;

    bool IsInside (const Vector2& point) const;
    void BoundingCircle (Vector2* point, float* radius) const;

    BBox Expand (float margin) const;
};

}
