#include "Property.h"

void Vector2Property::Save (string input) {
    stringstream stream(input);
    stream >> *m_v;
}

void Vector2Property::Load (string* out) const {
    stringstream stream;
    stream << *m_v;
    *out = stream.str();
}

void FloatProperty::Save (string input) {
    stringstream stream(input);
    stream >> *m_num;
}

void FloatProperty::Load (string* out) const {
    stringstream stream;
    stream << *m_num;
    *out = stream.str();
}

void IntProperty::Save (string input) {
    stringstream stream(input);
    stream >> *m_num;
}

void IntProperty::Load (string* out) const {
    stringstream stream;
    stream << *m_num;
    *out = stream.str();
}
