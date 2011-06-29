#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"

class ProjectileController;

class Projectile : public Volt::Entity {
public:
    Projectile ();
    virtual ~Projectile ();

    virtual void Update ();
    virtual void Render ();

private:
    ProjectileController* m_controller;
};
