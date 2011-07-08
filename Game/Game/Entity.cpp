#include "Entity.h"
#include "Game/Editor/Property.h"

void Entity::GetProperties (vector<Property*>* properties) {
    properties->push_back(
        new Vector2Property("Position", &m_transform.position));
    properties->push_back(new FloatProperty("Rotation", &m_transform.rotation));
    properties->push_back(new Vector2Property("Scale", &m_transform.scale));
    properties->push_back(new LayerProperty("Layer", this));
}
