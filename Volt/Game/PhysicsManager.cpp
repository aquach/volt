#include "Volt/Game/PhysicsManager.h"
#include "Volt/Game/Entity.h"
#include "Volt/Graphics/PhysicsDebugDraw.h"

namespace Volt {

static const int velocityIterations = 10;
static const int positionIterations = 10;

PhysicsManager* PhysicsManager::instance = NULL;

class ContactListener : public b2ContactListener {
public:
    virtual void BeginContact (b2Contact* contact) {
        const b2Body* bodyA = contact->GetFixtureA()->GetBody();
        const b2Body* bodyB = contact->GetFixtureB()->GetBody();
        Entity* entityA = (Entity*)bodyA->GetUserData();
        Entity* entityB = (Entity*)bodyB->GetUserData();
        CHECK_NOTNULL(entityA);
        CHECK_NOTNULL(entityB);

        entityA->BeginContact(entityB, contact);
        entityB->BeginContact(entityA, contact);
    }

    virtual void EndContact (b2Contact* contact) {
        const b2Body* bodyA = contact->GetFixtureA()->GetBody();
        const b2Body* bodyB = contact->GetFixtureB()->GetBody();
        Entity* entityA = (Entity*)bodyA->GetUserData();
        Entity* entityB = (Entity*)bodyB->GetUserData();
        CHECK_NOTNULL(entityA);
        CHECK_NOTNULL(entityB);

        entityA->EndContact(entityB, contact);
        entityB->EndContact(entityA, contact);
    }

    virtual void PreSolve (b2Contact* contact, const b2Manifold* oldManifold) {
        const b2Body* bodyA = contact->GetFixtureA()->GetBody();
        const b2Body* bodyB = contact->GetFixtureB()->GetBody();
        Entity* entityA = (Entity*)bodyA->GetUserData();
        Entity* entityB = (Entity*)bodyB->GetUserData();
        CHECK_NOTNULL(entityA);
        CHECK_NOTNULL(entityB);

        bool enabled = entityA->PreSolve(entityB);
        enabled &= entityB->PreSolve(entityA);
        contact->SetEnabled(enabled);
    }
};

class ContactFilter : public b2ContactFilter {
public:
    bool ShouldCollide (b2Fixture* fixtureA, b2Fixture* fixtureB) {
        const b2Body* bodyA = fixtureA->GetBody();
        const b2Body* bodyB = fixtureB->GetBody();
        Entity* entityA = (Entity*)bodyA->GetUserData();
        Entity* entityB = (Entity*)bodyB->GetUserData();
        CHECK_NOTNULL(entityA);
        CHECK_NOTNULL(entityB);

        return entityA->CanCollideWith(entityB) &&
               entityB->CanCollideWith(entityA);
    }
};

PhysicsManager::PhysicsManager ()
    : m_world(NULL),
      m_listener(NULL),
      m_filter(NULL),
      m_debugDraw(NULL) {
    b2Vec2 gravity(0.0f, 0.0f);
    bool doSleep = true;
    m_world = new b2World(gravity, doSleep);

    m_listener = new ContactListener;
    m_world->SetContactListener(m_listener);

    m_filter = new ContactFilter;
    m_world->SetContactFilter(m_filter);

    m_debugDraw = new PhysicsDebugDraw;
    m_world->SetDebugDraw(m_debugDraw);
}

PhysicsManager::~PhysicsManager () {
    delete m_world;
    delete m_listener;
    delete m_filter;
}

void PhysicsManager::Update () {
    m_world->Step(m_game->dt(), velocityIterations, positionIterations);
    m_world->ClearForces();
}

void PhysicsManager::Render () {
    m_world->DrawDebugData();
}

void PhysicsManager::SetGravity (Vector2 gravity) {
    m_world->SetGravity(gravity.ToB2());
}

Vector2 PhysicsManager::GetGravity () const {
    return m_world->GetGravity();
}

void PhysicsManager::SetDebugDraw (bool enabled) {
    uint32 flags = b2DebugDraw::e_shapeBit + b2DebugDraw::e_centerOfMassBit;/* +
                   b2DebugDraw::e_aabbBit;*/
    if (enabled)
        m_debugDraw->SetFlags(flags);
    else
        m_debugDraw->SetFlags(0);
}

}
