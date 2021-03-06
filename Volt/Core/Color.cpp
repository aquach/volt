#include "Volt/Core/Color.h"
#include "Volt/Core/Logging.h"
#include "Volt/Core/Random.h"

namespace Volt
{

Color Color::white    = Color (1,1,1,1);
Color Color::grey    = Color (0.5f, 0.5f, 0.5f, 1.0f);
Color Color::black    = Color (0, 0, 0, 1);
Color Color::red    = Color (1, 0, 0, 1);
Color Color::green    = Color (0, 1, 0, 1);
Color Color::blue    = Color (0, 0, 1, 1);
Color Color::yellow = Color (1, 1, 0, 1);
Color Color::orange = Color (1.0f, 0.5f, 0.0f, 1.0f);
Color Color::purple = Color (0.5f, 0.0f, 1.0f, 1.0f);

Color::Color (const Color &color)
    : r(color.r), g(color.g), b(color.b), a(color.a) {
}

Color Color::RGB (int r, int g, int b) {
    return Color((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
}

Color Color::RGBA (int r, int g, int b, int a) {
    return Color((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f,
                 (float)a / 255.0f);
}

Color Color::Random () {
    return Color(Random::Percent(), Random::Percent(), Random::Percent());
}

Color::Color ()
    : r(1), g(1), b(1), a(1) {

}

Color::Color (float r, float g, float b)
    : r(r), g(g), b(b), a(1.0f) {
}

Color::Color (float r, float g, float b, float a)
    : r(r), g(g), b(b), a(a) {
}

bool Color::operator!= (const Color& rhs) {
    return ((r != rhs.r) || (g != rhs.g) || (b != rhs.b) || (a != rhs.a));
}

void Color::Clamp () {
    for (unsigned int i = 0; i < 4; i++) {
        if ((*this)[i] > 1.0f)
            (*this)[i] = 1.0f;
        if ((*this)[i] < 0.0f)
            (*this)[i] = 0.0f;
    }
}

float& Color::operator[] (unsigned int i) {
    switch (i) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        default: return a;
    }
}

float Color::Intensity () const {
    return r * 0.3 + g * 0.59 + b * 0.11;
}

void Color::Load (const Json::Value& node) {
    CHECK(node.isMember("r"));
    CHECK(node.isMember("g"));
    CHECK(node.isMember("b"));
    CHECK(node.isMember("a"));
    r = node["r"].asDouble();
    g = node["g"].asDouble();
    b = node["b"].asDouble();
    a = node["a"].asDouble();
}

void Color::Save (Json::Value& node) const {
    node["r"] = r;
    node["g"] = g;
    node["b"] = b;
    node["a"] = a;
}

ostream& operator<< (ostream& stream, const Color& c) {
    stream << c.r << ", " << c.g << ", " << c.b << ", " << c.a;
    return stream;
}

istream& operator>> (istream& stream, Color& c) {
    char ch;
    Color temp;
    stream >> temp.r;
    stream >> ch;
    stream >> temp.g;
    stream >> ch;
    stream >> temp.b;
    stream >> ch;
    stream >> temp.a;
    if (!stream.fail())
        c = temp;
    return stream;
}

Color operator+ (const Color& lhs, const Color& rhs) {
    return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
}

Color operator- (const Color& lhs, const Color& rhs) {
    return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
}

Color operator* (float lhs, const Color& rhs) {
    return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
}

Color operator* (const Color& lhs, float rhs) {
    return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
}

Color operator/ (const Color& lhs, float rhs) {
    return Color(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs);
}

}
