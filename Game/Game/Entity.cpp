#include "Entity.h"

void Entity::GetProperties (vector<Property*>* properties) {
    properties->push_back(
        new Vector2Property("Position", &m_transform.position));
    //properties->push_back(new FloatProperty("Layer", &m_layer));
}
