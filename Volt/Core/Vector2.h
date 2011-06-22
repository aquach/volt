#pragma once

#include <ostream>

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

    Vector2 (const Vector2 &vector2);
    Vector2 ();
    Vector2 (float x, float y);

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
};

Vector2 operator+ (const Vector2& lhs, const Vector2& rhs);
Vector2 operator- (const Vector2& lhs, const Vector2& rhs);
Vector2 operator- (const Vector2& rhs);

Vector2 operator* (float lhs, const Vector2& rhs);
Vector2 operator* (const Vector2& lhs, float rhs);
Vector2 operator/ (const Vector2& lhs, float rhs);
Vector2 operator/ (float lhs, const Vector2& rhs);

Vector2 operator* (const Vector2& lhs, const Vector2& rhs);

ostream& operator<< (ostream& stream, const Vector2& other);

}
