#include "Entity.h"
#include "Game/Editor/Property.h"

void Entity::GetProperties (vector<Property*>* properties) {
    properties->push_back(new EntityPositionProperty("Position", this));
    properties->push_back(new EntityRotationProperty("Rotation", this));
    properties->push_back(new EntityScaleProperty("Scale", this));
    properties->push_back(new EntityLayerProperty("Layer", this));
}
