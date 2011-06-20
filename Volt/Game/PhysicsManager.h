#pragma once

#include "Core/Core.h"
#include <Box2D/Box2D.h>
#include "Game.h"

namespace Volt {

/**
 *  Wrapper manager for Box2D.
 */
class PhysicsManager {
public:
    PhysicsManager ();
    ~PhysicsManager ();

    void Update ();

    static void Register (PhysicsManager* manager) { instance = manager; }
    static PhysicsManager* Instance () {
        CHECK_NOTNULL(instance);
        return instance;
    }

    b2World* world () const { return m_world; }

private:
    friend class Game;
    Game* m_game;

    b2World* m_world;

    static PhysicsManager* instance;

    DISALLOW_COPY_AND_ASSIGN(PhysicsManager);
};

#define G_PhysicsManager PhysicsManager::Instance()

}
