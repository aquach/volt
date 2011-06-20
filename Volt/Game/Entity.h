#pragma once

#include <set>
#include <Box2D/Box2D.h>
#include "Core/Core.h"

/* A thing in the game with game logic. */
namespace Volt {

class Graphic;
class Scene;

class Entity {
public:
    Entity ();
    virtual ~Entity () { }

    int layer () const { return m_layer; }
    bool IsOnLayer (int layer) const { return m_layer == layer; }
    void SetLayer (int layer);

    void SetGraphic (Graphic* graphic);

    void SetParent (Entity* parent);
    Entity* parent () const { return m_parent; }

    Scene* scene () const { return m_scene; }

    bool HasTag (const string& tag) const {
        return m_tags.count(tag) == 1;
    }
    void AddTag (const string& tag) {
        m_tags.insert(tag);
    }
    void RemoveTag (const string& tag) {
        m_tags.erase(tag);
    }
    int numTags () const { return m_tags.size(); }

    virtual void Update () = 0;
    virtual void Render () = 0;

    virtual void OnAdded () { }

    void UpdatePhysics ();
    virtual void BeginContact (Entity* other) { }
    virtual void EndContact (Entity* other) { }
    virtual bool PreSolve (Entity* other) { return true; }
    virtual bool CanCollideWith (Entity* other) { return true; }

protected:
    b2Body* m_body;
    Transform m_transform;

private:
    friend class Scene;

    int m_layer;
    Scene* m_scene;
    Entity* m_parent;

    set<string> m_tags;

    DISALLOW_COPY_AND_ASSIGN(Entity);
};

}
