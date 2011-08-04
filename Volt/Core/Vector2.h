#pragma once

#include <ostream>
#include <json/json.h>
#include <Box2D/Box2D.h>

using namespace std;

namespace Volt {

class Vector2 {
public:
    float x;
    float y;

    static Vector2 zero;
    static Vector2 one;
    static Vector2 down;
    static Vector2 up;
    static Vector2 left;
    static Vector2 right;

    Vector2 (const Vector2& vector2);
    Vector2 (const b2Vec2& vec2);
    Vector2 ();
    Vector2 (float x, float y);

    b2Vec2 ToB2 ();

    void Set (float x, float y);
    void SetFromAngleDegrees (float angle);

    static Vector2 Random ();
    static Vector2 Reflect (const Vector2& a, const Vector2& b);

    float LengthSquared () const;
    float Length () const;
    Vector2 GetNormalized () const;
    void Normalize ();
    void Clamp (float max);
    bool IsInRange (float range) const;
    float DistanceTo (const Vector2& other) const;

    Vector2 GetPerpendicularLeft () const;
    Vector2 GetPerpendicularRight () const;

    float Dot (const Vector2& other) const;
    float Cross (const Vector2& other) const;
    Vector2 Reflect (const Vector2& other) const;

    Vector2 Project (const Vector2& other) const;
    Vector2 Rotate (float degrees) const;

    float GetAngleRadians () const;
    float GetAngleDegrees () const;

    float AngleTo (const Vector2& other) const;

    Vector2 xx () const;
    Vector2 yy () const;
    Vector2 yx () const;

    float& operator[] (unsigned int i);

    Vector2& operator= (const Vector2& rhs);
    bool operator== (const Vector2& rhs) const;
    bool operator!= (const Vector2& rhs) const;

    Vector2& operator+= (const Vector2& rhs);
    Vector2& operator-= (const Vector2& rhs);
    Vector2& operator*= (float rhs);
    Vector2& operator/= (float rhs);

    Vector2 operator+ (const Vector2& rhs) const;
    Vector2 operator- (const Vector2& rhs) const;
    Vector2 operator- () const;
    Vector2 operator* (float rhs) const;
    Vector2 operator/ (float rhs) const;
    Vector2 operator* (const Vector2& rhs) const;

    void Load (const Json::Value& node);
    void Save (Json::Value& node) const;
};

inline Vector2::Vector2 (const Vector2 &vector2)
    : x(vector2.x), y(vector2.y) {
}

inline Vector2::Vector2 (const b2Vec2 &vec2)
    : x(vec2.x), y(vec2.y) {
}

inline Vector2::Vector2 ()
    : x(0.0f), y(0.0f) {
}

inline Vector2::Vector2 (float x, float y)
    : x(x), y(y) {
}

inline Vector2& Vector2::operator= (const Vector2& rhs){
    if (&rhs != this) {
        x = rhs.x;
        y = rhs.y;
    }
    return *this;
}

inline bool Vector2::operator== (const Vector2& rhs) const {
    return ((x == rhs.x) && (y == rhs.y));
}

inline bool Vector2::operator!= (const Vector2& rhs) const {
    return ((x != rhs.x) || (y != rhs.y));
}

inline Vector2& Vector2::operator+= (const Vector2& rhs) {
    x = x + rhs.x;
    y = y + rhs.y;
    return *this;
}

inline Vector2& Vector2::operator-= (const Vector2& rhs) {
    x = x - rhs.x;
    y = y - rhs.y;
    return *this;
}

inline Vector2& Vector2::operator*= (float rhs) {
    x = x * rhs;
    y = y * rhs;
    return *this;
}

inline Vector2& Vector2::operator/= (float rhs) {
    x = x / rhs;
    y = y / rhs;
    return *this;
}

inline Vector2 Vector2::operator+ (const Vector2& rhs) const {
    return Vector2(x + rhs.x, y + rhs.y);
}

inline Vector2 Vector2::operator- (const Vector2& rhs) const {
    return Vector2(x - rhs.x, y - rhs.y);
}

inline Vector2 Vector2::operator- () const {
    return Vector2(-x, -y);
}

inline Vector2 operator* (float lhs, const Vector2& rhs) {
    return Vector2(lhs * rhs.x, lhs * rhs.y);
}

inline Vector2 Vector2::operator* (float rhs) const {
    return Vector2(x * rhs, y * rhs);
}

inline Vector2 Vector2::operator/ (float rhs) const {
    return Vector2(x / rhs, y / rhs);
}

inline Vector2 Vector2::operator* (const Vector2& rhs) const {
    return Vector2(x * rhs.x, y * rhs.y);
}

inline Vector2 operator/ (float lhs, const Vector2 &rhs) {
    return Vector2(lhs / rhs.x, lhs / rhs.y);
}

ostream& operator<< (ostream& stream, const Vector2& other);
istream& operator>> (istream& stream, Vector2& other);
}
