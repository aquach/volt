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

private:
    friend class Game;
    Game* m_game;

    b2World* m_world;
    b2Body* body;
    DISALLOW_COPY_AND_ASSIGN(PhysicsManager);
};

}
