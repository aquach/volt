#include "Rect.h"
#include "Macros.h"

namespace Volt
{
    Rect::Rect () {
    }

    Rect::Rect (const Vector2& min, const Vector2& max):
        min(min), max(max) {
    }

    Rect::Rect (const Rect& other):
        min(other.min), max(other.max) {
    }

    void Rect::UnionWith (const Rect& other) {
        min.x = MIN(other.min.x, min.x);
        min.y = MIN(other.min.y, min.y);
        max.x = MIN(other.max.x, max.x);
        max.y = MIN(other.max.y, max.y);
    }
}
