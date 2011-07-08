#pragma once

#include "Game/Core/Core.h"

class Entity;

class VelocityController {
public:
    VelocityController (Entity* entity, Vector2 targetVel)
        : m_entity(entity),
          m_targetVel(targetVel),
          m_impulse(Vector2(2, 2)) {
    }

    void Update ();

    void SetTargetVelocity (Vector2 vel) { m_targetVel = vel; }
    Vector2 targetVelocity () const { return m_targetVel; }

    void SetImpulse (Vector2 impulse) { m_impulse = impulse; }
    Vector2 impulse () const { return m_impulse; }

private:
    Entity* m_entity;
    Vector2 m_targetVel;
    Vector2 m_impulse;
};
