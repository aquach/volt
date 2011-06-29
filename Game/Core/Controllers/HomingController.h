#pragma once

#include "Core/Core.h"
#include "ProjectileController.h"

namespace Volt {
    class Entity;
}

/* Controls an entity's velocity to hit the target entity. */
class HomingController {
public:
    HomingController (Volt::Entity* entity, Volt::Entity* target,
                      const ProjectileControllerDef& def)
        : m_target(target),
          m_projectileController(entity, Vector2::zero, def) {
    }

    void Update ();

private:
    ProjectileController m_projectileController;
    Volt::Entity* m_target;
};
