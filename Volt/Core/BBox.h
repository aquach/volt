#pragma once

#include <json/json.h>
#include "Volt/Core/Macros.h"
#include "Volt/Core/Math.h"
#include "Volt/Core/Vector2.h"

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

    Vector2 center () const {
        return (min + max) * 0.5f;
    }

    Vector2 extents () const {
        return max - min;
    }

    BBox Union (const BBox& other) const;
    BBox Union (const Vector2& point) const;

    bool IsInside (const Vector2& point) const;
    void BoundingCircle (Vector2* point, float* radius) const;

    bool Intersects (const BBox& other) const;

    BBox Expand (float margin) const;

    void Load (const Json::Value& node);
    void Save (Json::Value& node) const;
};

ostream& operator<< (ostream& stream, const BBox& box);

}
