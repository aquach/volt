#pragma once

#include "Core/Core.h"

namespace Volt {

class Entity;

/**
 *  A scene of the game. Scenes can be startup screens, level screens, gameplay,
    etc.
 */
class Scene {
public:
    Scene ();
    virtual ~Scene () { }

    virtual void Render ();
    virtual void Update ();

    virtual void OnBegin () { };
    virtual void OnEnd () { };

    void Add (Entity* entity);
    void Remove (Entity* entity);

private:
    void ResolveEntityChanges ();

    bool m_isPaused;

    typedef map<int, list<Entity* > > Layers;
    Layers m_layers;
    list<Entity*> m_entitiesToAdd;
    list<Entity*> m_entitiesToRemove;

    DISALLOW_COPY_AND_ASSIGN(Scene);
};

}
