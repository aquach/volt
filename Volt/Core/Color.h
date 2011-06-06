#pragma once

namespace Volt
{

/**
 * Describes a color using 4 floats in 0.0 - 1.0 range.
 */
class Color
{
public:
	Color (const Color &color);
	Color ();
	Color (float r, float g, float b, float a);

	void Clamp ();

	float& operator[] (unsigned int i);
	bool operator!= (const Color& rhs);

	float r, g, b, a;

	static Color white;
	static Color grey;
	static Color black;
	static Color red;
	static Color green;
	static Color blue;
	static Color yellow;
	static Color orange;
	static Color purple;
};

Color operator+ (const Color& lhs, const Color& rhs);
Color operator- (const Color& lhs, const Color& rhs);
Color operator* (float lhs, const Color& rhs);
Color operator* (const Color& lhs, float rhs);
Color operator/ (const Color& lhs, float rhs);

}
