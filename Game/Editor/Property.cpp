#include "Property.h"
#include "Game/Game/Entity.h"

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

void EntityLayerProperty::Save (string input) {
    stringstream stream(input);
    int layer;
    stream >> layer;
    if (stream.fail())
        return;
    m_entity->SetLayer(layer);
}

void EntityLayerProperty::Load (string* out) const {
    stringstream stream;
    stream << m_entity->layer();
    *out = stream.str();
}

void EntityPositionProperty::Save (string input) {
    stringstream stream(input);
    Vector2 position;
    stream >> position;
    if (stream.fail())
        return;
    m_entity->SetPosition(position);
}

void EntityPositionProperty::Load (string* out) const {
    stringstream stream;
    stream << m_entity->position();
    *out = stream.str();
}

void EntityRotationProperty::Save (string input) {
    stringstream stream(input);
    float rotation;
    stream >> rotation;
    if (stream.fail())
        return;
    m_entity->SetRotation(rotation);
}

void EntityRotationProperty::Load (string* out) const {
    stringstream stream;
    stream << m_entity->rotation();
    *out = stream.str();
}

void EntityScaleProperty::Save (string input) {
    stringstream stream(input);
    Vector2 scale;
    stream >> scale;
    if (stream.fail())
        return;
    m_entity->SetScale(scale);
}

void EntityScaleProperty::Load (string* out) const {
    stringstream stream;
    stream << m_entity->scale();
    *out = stream.str();
}
