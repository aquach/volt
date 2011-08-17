#include "Entity.h"
#include "Game/Editor/Property.h"

void Entity::GetProperties (vector<Property*>* properties) {
    properties->push_back(new EntityPositionProperty("Position", this));
    properties->push_back(new EntityRotationProperty("Rotation", this));
    properties->push_back(new EntityScaleProperty("Scale", this));
    properties->push_back(new EntityLayerProperty("Layer", this));
    properties->push_back(new BoolProperty("Occludes Light", &m_occludesLight));
}

void Entity::CopyFrom (const Entity* other) {
    Volt::Entity::CopyFrom(other);
    m_occludesLight = other->m_occludesLight;
}

void Entity::Load (const Json::Value& node) {
    Volt::Entity::Load(node);
    m_occludesLight = node.get("occludesLight", true).asBool();
}

void Entity::Save (Json::Value& node) const {
    Volt::Entity::Save(node);
    node["occludesLight"] = m_occludesLight;
    node["type"] = m_type;
    if (m_type == "")
        LOG(WARNING) << "Saving Entity " << *this << " with no type.";
}

void Entity::OnAccessed (Entity* accessor) {
    FOR_(set<EntityAccessListener*>::iterator, i, m_accessListeners) {
        (*i)->OnAccessed(accessor);
    }
}

ostream& Entity::ToString (ostream& stream) const {
    stream << "Game ";
    return Volt::Entity::ToString(stream);
}

ostream& operator<< (ostream& stream, const Entity& e) {
    return e.ToString(stream);
}
