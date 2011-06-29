#pragma once

#include "Core/Core.h"

namespace Volt {
    class Entity;
}

struct ProjectileControllerDef {
    ProjectileControllerDef ()
        : maxLinearSpeed(5),
          linearP(1),
          linearD(0.1),
          maxRotationSpeed(180),
          rotationP(3),
          rotationD(0.1) {
    }

    float maxLinearSpeed;
    float linearP;
    float linearD;

    // Degrees.
    float maxRotationSpeed;
    float rotationP;
    float rotationD;
};

/* Controls an entity's velocity to act like a projectile. Projectiles can only
 * move in the direction they're facing and usually have finite turning
 * speed. Uses PD controllers. Assumes x is the lookAt direction of the
 * projectile. */
class ProjectileController {
public:
    ProjectileController (Volt::Entity* entity, Vector2 targetPos,
                          const ProjectileControllerDef& def)
        : m_targetPos(targetPos),
          m_entity(entity),
          m_def(def) {
    }

    void Update ();

private:
    Volt::Entity* m_entity;
    Vector2 m_targetPos;
    ProjectileControllerDef m_def;
};
