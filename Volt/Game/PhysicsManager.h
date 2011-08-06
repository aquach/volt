#pragma once

#include "Volt/Core/Core.h"
#include <Box2D/Box2D.h>
#include "Volt/Game/Game.h"

namespace Volt {

/**
 *  Wrapper manager for Box2D.
 */
class PhysicsManager {
public:
    PhysicsManager ();
    ~PhysicsManager ();

    void Update ();
    void Render ();

    static void Register (PhysicsManager* manager) { instance = manager; }
    static PhysicsManager* Instance () { return instance; }

    b2World* world () const { return m_world; }

    void SetGravity (Vector2 gravity);
    Vector2 GetGravity () const;

    void SetDebugDraw (bool enabled);
    void ToggleDebugDraw ();

    void LockWorld ();
    void UnlockWorld ();

private:
    friend class Game;
    Game* m_game;

    b2World* m_world;
    b2ContactListener* m_listener;
    b2ContactFilter* m_filter;
    b2DebugDraw* m_debugDraw;
    bool m_debugDrawEnabled;
    Lock m_worldLock;

    static PhysicsManager* instance;

    DISALLOW_COPY_AND_ASSIGN(PhysicsManager);
};

#define G_PhysicsManager PhysicsManager::Instance()

}
