#pragma once

#include <set>
#include <Box2D/Box2D.h>
#include "Volt/Core/Core.h"

/* A thing in the game with game logic. */
namespace Volt {

class Entity;
class Graphic;
class Scene;

class EntityContactListener {
public:
    virtual void OnContactBegin (Entity* other, b2Contact* contact) = 0;
    virtual void OnContactEnd (Entity* other, b2Contact* contact) = 0;
};

class Entity {
public:
    Entity ();
    virtual ~Entity ();

    int layer () const { return m_layer; }
    bool IsOnLayer (int layer) const { return m_layer == layer; }
    void SetLayer (int layer);

    /* Clone should be overriden in child classes that are clonable. They should
     * create an instance of the child class, then use CopyFrom to copy each
     * slice of the class into the new instance. */
    virtual Entity* Clone () const { return NULL; };
    void CopyFrom (const Entity* other);

    Scene* scene () const { return m_scene; }

    bool HasTag (const string& tag) const {
        return m_tags.count(tag) == 1;
    }
    void AddTag (const string& tag);
    void RemoveTag (const string& tag);
    void GetTags (vector<string>* tags) const {
        tags->resize(m_tags.size());
        copy(m_tags.begin(), m_tags.end(), tags->begin());
    }
    int numTags () const { return m_tags.size(); }

    virtual void Update () = 0;
    virtual void Render () = 0;

    virtual void OnAdded () { }
    virtual void OnRemoved () { }

    void UpdatePhysics ();
    b2Body* CreateBody (b2BodyDef def);
    b2Body* CreateBody (b2BodyType type);
    void DestroyBody ();
    b2Body* body () const { return m_body; }

    // These are called when entities enter and leave the bounding boxes of
    // other entities. Do not call these directly.
    void BeginContact (Entity* other, b2Contact* contact);
    void EndContact (Entity* other, b2Contact* contact);

    // This is called every frame that this entity touches another entity.
    // Returns true if the contact should be registered.
    virtual bool PreSolve (Entity* other) { return true; }
    virtual bool CanCollideWith (Entity* other) { return true; }

    void RemoveSelf ();

    Transform transform () const { return m_transform; }
    Vector2 position () const { return m_transform.position; }
    void SetPosition (Vector2 pos);
    float rotation () const { return m_transform.rotation; }
    void SetRotation (float rotation);
    Vector2 scale () const { return m_transform.scale; }
    void SetScale (Vector2 scale);

    virtual void OnScaleChanged () { }

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    void AddContactListener (EntityContactListener* listener);
    void RemoveContactListener (EntityContactListener* listener);

protected:
    b2Body* m_body;
    Transform m_transform;
    // Direction of last movement (only works with physics).
    Vector2 m_moveDirection;

private:
    friend class Scene;
    friend ostream& operator<< (ostream& stream, const Entity& e);

    int m_layer;
    Scene* m_scene;

    set<string> m_tags;
    set<EntityContactListener*> m_contactListeners;

    DISALLOW_COPY_AND_ASSIGN(Entity);
};

ostream& operator<< (ostream& stream, const Entity& e);

}
