#pragma once

#include "Game/Core/Core.h"

namespace Volt {
    class Entity;
}

class RotationController {
public:
    RotationController (Volt::Entity* entity, float targetRotation)
        : m_entity(entity),
          m_targetRotation(targetRotation),
          m_p(2),
          m_d(0.2),
          m_maxSpeed(3) {
    }

    void Update ();

    void SetTargetRotation (float rotation) { m_targetRotation = rotation; }
    float targetRotation () const { return m_targetRotation; }

    void SetP (float p) { m_p = p; }
    float p () const { return m_p; }

    void SetD (float d) { m_d = d; }
    float d () const { return m_d; }
    void SetMaxSpeed (float s) { m_maxSpeed = s; }
    float maxSpeed () const { return m_maxSpeed; }

private:
    Volt::Entity* m_entity;
    float m_targetRotation;
    float m_p;
    float m_d;
    float m_maxSpeed;
};
