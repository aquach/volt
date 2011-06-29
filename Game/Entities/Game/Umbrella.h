#pragma once

#include "Core/Core.h"
#include "Weapon.h"

class Umbrella : public Weapon {
public:
    Umbrella ();
    virtual ~Umbrella ();

    virtual void OnFire ();
    virtual void OnGuard ();
};
