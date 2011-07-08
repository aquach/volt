#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"

class ProjectileController;

class Projectile : public Entity {
public:
    Projectile ();
    virtual ~Projectile ();

    virtual void Update ();
    virtual void Render ();

private:
    ProjectileController* m_controller;
};
