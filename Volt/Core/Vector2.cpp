#include "Vector2.h"
#include "Logging.h"
#include "Random.h"
#include "Math.h"
#include <cmath>

namespace Volt {

Vector2 Vector2::zero = Vector2 (0,0);
Vector2 Vector2::one = Vector2 (1,1);
Vector2 Vector2::up = Vector2 (0, -1);
Vector2 Vector2::down = Vector2 (0, 1);
Vector2 Vector2::left = Vector2 (-1, 0);
Vector2 Vector2::right = Vector2 (1, 0);

Vector2::Vector2 (const Vector2 &vector2)
    : x(vector2.x), y(vector2.y) {
}

Vector2::Vector2 (const b2Vec2 &vec2)
    : x(vec2.x), y(vec2.y) {
}

Vector2::Vector2 ()
    : x(0.0f), y(0.0f) {
}

Vector2::Vector2 (float x, float y)
    : x(x), y(y) {
}

b2Vec2 Vector2::ToB2 () {
    return b2Vec2(x, y);
}

void Vector2::Set (float x, float y)
{
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

float& Vector2::operator[] (unsigned int i) {
    switch(i) {
        case 0: return x;
        case 1: return y;
        default: return x;
    }
}

Vector2& Vector2::operator= (const Vector2& rhs){
    if (&rhs != this) {
        x = rhs.x;
        y = rhs.y;
    }
    return *this;
}

bool Vector2::operator== (const Vector2& rhs) const {
    return ((x == rhs.x) && (y == rhs.y));
}

bool Vector2::operator!= (const Vector2& rhs) const {
    return ((x != rhs.x) || (y != rhs.y));
}

Vector2& Vector2::operator+= (const Vector2& rhs) {
    x = x + rhs.x;
    y = y + rhs.y;
    return *this;
}

Vector2& Vector2::operator-= (const Vector2& rhs) {
    x = x - rhs.x;
    y = y - rhs.y;
    return *this;
}

Vector2& Vector2::operator*= (float rhs) {
    x = x * rhs;
    y = y * rhs;
    return *this;
}

Vector2& Vector2::operator/= (float rhs) {
    x = x / rhs;
    y = y / rhs;
    return *this;
}

Vector2 operator+ (const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2 operator- (const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2 operator- (const Vector2 &rhs) {
    return Vector2(-rhs.x, -rhs.y);
}

Vector2 operator* (float lhs, const Vector2& rhs) {
    return Vector2(lhs * rhs.x, lhs * rhs.y);
}

Vector2 operator* (const Vector2& lhs, float rhs) {
    return Vector2(lhs.x * rhs, lhs.y * rhs);
}

Vector2 operator/ (const Vector2& lhs, float rhs) {
    return Vector2(lhs.x / rhs, lhs.y / rhs);
}

Vector2 operator* (const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
}

Vector2 operator/ (float lhs, const Vector2 &rhs) {
    return Vector2(lhs / rhs.x, lhs / rhs.y);
}

ostream& operator<< (ostream& stream, const Vector2& other) {
    stream << "[" << other.x << ", " << other.y << "]";
    return stream;
}

void Vector2::Load (const Json::Value& node) {
    CHECK(node.size() == 2);
    x = node[0u].asDouble();
    y = node[1u].asDouble();
}

void Vector2::Save (Json::Value& node) const {
    node.append(x);
    node.append(y);
}

}
