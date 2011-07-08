#pragma once

#include "Game/Core/Core.h"
#include "Game/Core/Controllers/ProjectileController.h"

class Entity;

/* Controls an entity's velocity to hit the target entity. */
class HomingController {
public:
    HomingController (Entity* entity, Entity* target,
                      const ProjectileControllerDef& def)
        : m_target(target),
          m_projectileController(entity, Vector2::zero, def) {
    }

    void Update ();

private:
    ProjectileController m_projectileController;
    Entity* m_target;
};
