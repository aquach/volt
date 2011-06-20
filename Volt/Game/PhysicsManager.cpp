#include "PhysicsManager.h"

namespace Volt {

static const int velocityIterations = 10;
static const int positionIterations = 10;

PhysicsManager* PhysicsManager::instance = NULL;

PhysicsManager::PhysicsManager () {
    b2Vec2 gravity(0.0f, 0.0f);
    bool doSleep = true;
    m_world = new b2World(gravity, doSleep);
}

PhysicsManager::~PhysicsManager () {
    delete m_world;
}

void PhysicsManager::Update () {
    m_world->Step(m_game->dt(), velocityIterations, positionIterations);
    m_world->ClearForces();
}

}
