#include "Entity.h"
#include "Game/Editor/Property.h"

void Entity::GetProperties (vector<Property*>* properties) {
    properties->push_back(new EntityPositionProperty("Position", this));
    properties->push_back(new EntityRotationProperty("Rotation", this));
    properties->push_back(new EntityScaleProperty("Scale", this));
    properties->push_back(new EntityLayerProperty("Layer", this));
    //properties->push_back(new BoolProperty("Occludes Light", &m_occludesLight));
}

void Entity::CopyFrom (const Entity* other) {
    Entity::CopyFrom(other);
    m_occludesLight = other->m_occludesLight;
}

void Entity::Load (const Json::Value& node) {
    Volt::Entity::Load(node);
    m_occludesLight = node.get("occludesLight", true).asBool();
}

void Entity::Save (Json::Value& node) const {
    Volt::Entity::Save(node);
    node["occludesLight"] = m_occludesLight;
}

