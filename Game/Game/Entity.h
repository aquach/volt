#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/Entity.h"

class Entity;
class Property;

class EntityAccessListener {
public:
    virtual ~EntityAccessListener () { }
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

    void SetType (string type) { m_type = type; }
    const string& type () const { return m_type; }

private:
    /* The type of the Entity which is will be saved in level files and will
     * be used to differentiate and create Entities. For Entities, this should
     * be the name of the class, and for Python Entities, it should be the ID.
     */
    string m_type;
    set<EntityAccessListener*> m_accessListeners;
    bool m_occludesLight;
};

ostream& operator<< (ostream& stream, const Entity& e);
