#include "PhysicsManager.h"
#include "Entity.h"

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

        entityA->BeginContact(entityB);
        entityB->BeginContact(entityA);
    }

    virtual void EndContact (b2Contact* contact) {
        const b2Body* bodyA = contact->GetFixtureA()->GetBody();
        const b2Body* bodyB = contact->GetFixtureB()->GetBody();
        Entity* entityA = (Entity*)bodyA->GetUserData();
        Entity* entityB = (Entity*)bodyB->GetUserData();
        CHECK_NOTNULL(entityA);
        CHECK_NOTNULL(entityB);

        entityA->EndContact(entityB);
        entityB->EndContact(entityA);
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
      m_filter(NULL) {
    b2Vec2 gravity(0.0f, 0.0f);
    bool doSleep = true;
    m_world = new b2World(gravity, doSleep);

    m_listener = new ContactListener;
    m_world->SetContactListener(m_listener);

    m_filter = new ContactFilter;
    m_world->SetContactFilter(m_filter);
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

}
