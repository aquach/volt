#pragma once

#include "Core/Core.h"

namespace Volt {

/**
 * Describes a color using 4 floats in 0.0 - 1.0 range.
 */
class Color
{
public:
    Color (const Color &color);
    Color ();
    Color (float r, float g, float b);
    Color (float r, float g, float b, float a);

    static Color RGB (int r, int g, int b);
    static Color RGBA (int r, int g, int b, int a);

    void Clamp ();

    float& operator[] (unsigned int i);
    bool operator!= (const Color& rhs);

    void Load (const Json::Value& node);
    void Save (Json::Value& node) const;

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
