#include "Volt/Core/Transform.h"
#include "Volt/Core/Logging.h"
#include "Volt/Core/Math.h"

namespace Volt {

Transform::Transform ()
    : position(Vector2::zero),
      rotation(0.0f),
      scale(Vector2::one) {
}

Transform::Transform (const Transform &transform)
    : position(transform.position),
      rotation(transform.rotation),
      scale(transform.scale) {
}

void Transform::LerpTransform (Transform *prev, Transform *next, float percent)
{
    position = next->position * percent + prev->position * (1.0f - percent);
    rotation = next->rotation * percent + prev->rotation * (1.0f - percent);
    scale = next->scale * percent + prev->scale * (1.0f - percent);
}

Vector2 Transform::GetDirectionVector() const {
    return Vector2(sin(rotation * DEG2RAD), -cos(rotation * DEG2RAD));
}

ostream& operator<< (ostream& stream, const Transform& other) {
    stream << other.position << " rot " << other.rotation << " * "
           << other.scale;
    return stream;
}

void Transform::Load (const Json::Value& node) {
    CHECK(node.isMember("position"));
    CHECK(node.isMember("rotation"));
    CHECK(node.isMember("scale"));

    position.Load(node["position"]);
    rotation = node["rotation"].asDouble();
    scale.Load(node["scale"]);
}

void Transform::Save (Json::Value& node) const {
    position.Save(node["position"]);
    node["rotation"] = rotation;
    scale.Save(node["scale"]);
}

Vector2 Transform::Apply (Vector2 v) const {
    v.x *= scale.x;
    v.y *= scale.y;
    v = v.Rotate(rotation);
    return v + position;
}

Vector2 Transform::ApplyInverse (Vector2 v) const {
    v -= position;
    v = v.Rotate(-rotation);
    v.x /= scale.x;
    v.y /= scale.y;
    return v;
}

Transform Transform::Multiply (const Transform& other) const {
    /* Scaling not supported. It creates transformations not representable by
       position/rotation/scale. */
    CHECK(other.scale.x == 1 && other.scale.y == 1);
    Transform t;
    Vector2 v = other.position.Rotate(rotation);
    t.position = position + Vector2(v.x * scale.x, v.y * scale.y);
    t.rotation = rotation + other.rotation;
    t.scale = scale;
    return t;
}

}
