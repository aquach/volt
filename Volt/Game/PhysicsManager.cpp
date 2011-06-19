#include "PhysicsManager.h"

namespace Volt {

static const int velocityIterations = 10;
static const int positionIterations = 10;

PhysicsManager::PhysicsManager () {
    b2Vec2 gravity(0.0f, -10.0f);
    bool doSleep = true;
    m_world = new b2World(gravity, doSleep);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    body = m_world->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
}

PhysicsManager::~PhysicsManager () {
    delete m_world;
}

void PhysicsManager::Update () {
    m_world->Step(m_game->dt(), velocityIterations, positionIterations);
    m_world->ClearForces();
}

}
