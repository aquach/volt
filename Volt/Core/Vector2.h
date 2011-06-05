#pragma once

namespace Volt
{
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

		Vector2(const Vector2 &vector2);
		Vector2();
		Vector2(float x, float y);

        void Set(float x, float y);

		static Vector2 Random();
		static Vector2 Reflect(const Vector2& a, const Vector2& b);

		float LengthSquared();
		float Length();
		Vector2 GetNormalized();
		void Normalize();
		Vector2 GetPerpendicularLeft();
		Vector2 GetPerpendicularRight();
		void Clamp(float max);
		bool IsInRange(float range);
		float Dot(Vector2 other);
		float Cross(Vector2 other);
		Vector2 Reflect(const Vector2 &other);

		float GetAngleRadians();
		float GetAngleDegrees();

		Vector2 xx() const;
		Vector2 yy() const;
		Vector2 yx() const;

		float& operator[] (unsigned int i);

		Vector2& operator=(const Vector2& rhs);
		bool operator==(const Vector2& rhs);
		bool operator!=(const Vector2& rhs);

		Vector2& operator+=(const Vector2& rhs);
		Vector2& operator-=(const Vector2& rhs);
		Vector2& operator*=(float rhs);
		Vector2& operator/=(float rhs);

	};

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
	Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
	Vector2 operator-(const Vector2 &rhs);

	Vector2 operator*(float lhs, const Vector2& rhs);
	Vector2 operator*(const Vector2& lhs, float rhs);
	Vector2 operator/(const Vector2& lhs, float rhs);
	Vector2 operator/(float lhs, const Vector2 &rhs);

	Vector2 operator*(const Vector2& lhs, const Vector2& rhs);
}
