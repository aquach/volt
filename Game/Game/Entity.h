#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/Entity.h"

class Entity;
class Property;

class EntityAccessListener {
public:
    virtual void OnAccessed (Entity* accessor) = 0;
};

class Entity : public Volt::Entity {
public:
    Entity () : m_occludesLight(true) { }
    virtual ~Entity () { }

    virtual void GetProperties (vector<Property*>* properties);

    void SetOccludesLight (bool enabled) { m_occludesLight = enabled; }
    bool occludesLight () const { return m_occludesLight; }

    void OnAccessed (Entity* accessor);

    void AddAccessListener (EntityAccessListener* listener) {
        m_accessListeners.insert(listener);
    }
    void RemoveAccessListener (EntityAccessListener* listener) {
        m_accessListeners.erase(listener);
    }

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    void CopyFrom (const Entity* other);

    virtual ostream& ToString (ostream& stream) const;

private:
    set<EntityAccessListener*> m_accessListeners;
    bool m_occludesLight;
};

ostream& operator<< (ostream& stream, const Entity& e);
