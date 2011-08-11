#include "Volt/Core/Vector2.h"
#include "Volt/Core/Logging.h"
#include "Volt/Core/Random.h"
#include "Volt/Core/Math.h"
#include <cmath>

namespace Volt {

Vector2 Vector2::zero = Vector2 (0,0);
Vector2 Vector2::one = Vector2 (1,1);
Vector2 Vector2::up = Vector2 (0, -1);
Vector2 Vector2::down = Vector2 (0, 1);
Vector2 Vector2::left = Vector2 (-1, 0);
Vector2 Vector2::right = Vector2 (1, 0);

b2Vec2 Vector2::ToB2 () {
    return b2Vec2(x, y);
}

void Vector2::Set (float x) {
    this->x = x, this->y = x;
}

void Vector2::Set (float x, float y) {
    this->x = x, this->y = y;
}

void Vector2::SetFromAngleDegrees (float angle) {
    x = cos(angle * Volt::DEG2RAD);
    y = sin(angle * Volt::DEG2RAD);
}

Vector2 Vector2::Random () {
    float rad = (Random::Range(0, 360)) * Volt::DEG2RAD;
    return Vector2(sin(rad), cos(rad));
}

float Vector2::DistanceTo (const Vector2& other) const {
    return (other - *this).Length();
}

float Vector2::LengthSquared () const {
    return x * x + y * y;
}

float Vector2::Length () const {
    return sqrt(x * x + y * y);
}

Vector2 Vector2::GetNormalized () const {
    float mag = Length();
    return Vector2(x / mag, y / mag);
}

void Vector2::Normalize () {
    float mag = Length();
    x /= mag;
    y /= mag;
}

Vector2 Vector2::GetPerpendicularLeft () const {
    return Vector2(-y, x);
}

Vector2 Vector2::GetPerpendicularRight () const {
    return Vector2(y, -x);
}

void Vector2::Clamp (float max) {
    if (x * x + y * y > max * max)
    {
        Normalize();
        x *= max;
        y *= max;
    }
}

bool Vector2::IsInRange (float range) const {
    return (x * x + y * y) <= range * range;
}

float Vector2::Dot (const Vector2& b) const {
    return (x * b.x + y * b.y);
}

float Vector2::Cross (const Vector2& b) const {
    return (x * b.y - y * b.x);
}

Vector2 Vector2::Reflect (const Vector2& a, const Vector2& b) {
    Vector2 newVec;
    float dotProduct = -a.Dot(b);
    newVec.x = a.x + 2 * b.x * dotProduct;
    newVec.y = a.y + 2 * b.y * dotProduct;
    return newVec;
}

Vector2 Vector2::Project (const Vector2& other) const {
    return Dot(other) / other.LengthSquared() * other;
}

Vector2 Vector2::Rotate (float degrees) const {
    return Vector2(
        x * cosf(degrees * DEG2RAD) - y * sinf(degrees * DEG2RAD),
        x * sinf(degrees * DEG2RAD) + y * cosf(degrees * DEG2RAD));
}

Vector2 Vector2::Reflect (const Vector2& other) const {
    return Vector2::Reflect(*this, other);
}

float Vector2::GetAngleRadians () const {
    float angle = atan2(y, x);
    if (angle < 0)
    {
        angle += 2 * Volt::PI;
    }
    return angle;
}

float Vector2::GetAngleDegrees () const {
    return GetAngleRadians() * Volt::RAD2DEG;
}

Vector2 Vector2::xx () const {
    return Vector2(this->x, this->x);
}

Vector2 Vector2::yy () const {
    return Vector2(this->y, this->y);
}

Vector2 Vector2::yx () const {
    return Vector2(this->y, this->x);
}

float Vector2::AngleTo (const Vector2& other) const {
    float angle = atan2(other.y, other.x) - atan2(y, x);
    if (angle < -PI)
        angle += 2 * PI;
    return angle * RAD2DEG;
}

float& Vector2::operator[] (unsigned int i) {
    switch(i) {
        case 0: return x;
        case 1: return y;
        default: return x;
    }
}

ostream& operator<< (ostream& stream, const Vector2& other) {
    stream << "[" << other.x << ", " << other.y << "]";
    return stream;
}

istream& operator>> (istream& stream, Vector2& other) {
    char ch;
    stream >> ch;
    stream >> other.x;
    stream >> ch;
    stream >> other.y;
    stream >> ch;
    return stream;
}

void Vector2::Load (const Json::Value& node) {
    CHECK(node.size() == 2);
    x = (float)node[0u].asDouble();
    y = (float)node[1u].asDouble();
}

void Vector2::Save (Json::Value& node) const {
    node.append(x);
    node.append(y);
}

}
