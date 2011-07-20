#pragma once

#include "Volt/Game/Entity.h"

class Property;

class Entity : public Volt::Entity {
public:
    Entity () { }
    virtual ~Entity () { }

    virtual void OnAccessed (Entity* accessor) { }

    virtual void GetProperties (vector<Property*>* properties);

    void SetOccludesLight (bool enabled) { m_occludesLight = enabled; }
    bool occludesLight () const { return m_occludesLight; }

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    void CopyFrom (const Entity* other);

private:
    bool m_occludesLight;
};
