#include "BBox.h"
#include "Macros.h"

namespace Volt
{

BBox BBox::Union (const BBox& other) const {
    BBox ret;
    ret.min.x = MIN(other.min.x, min.x);
    ret.min.y = MIN(other.min.y, min.y);
    ret.max.x = MAX(other.max.x, max.x);
    ret.max.y = MAX(other.max.y, max.y);
    return ret;
}

BBox BBox::Union (const Vector2& point) const {
    BBox ret;
    ret.min.x = MIN(min.x, point.x);
    ret.min.y = MIN(min.y, point.y);
    ret.max.x = MAX(max.x, point.x);
    ret.max.y = MAX(max.y, point.y);
    return ret;
}

bool BBox::IsInside (const Vector2& point) const {
    return (point.x >= min.x && point.x <= max.x) &&
           (point.y >= min.y && point.y < max.y);
}

void BBox::BoundingCircle (Vector2* point, float* radius) const {
    *point = 0.5f * min + 0.5f * max;
    *radius = point->DistanceTo(max);
}

BBox BBox::Expand (float margin) const {
    return BBox(
        Vector2(min.x - margin, min.y - margin),
        Vector2(max.x + margin, max.y + margin)
    );
}

}
