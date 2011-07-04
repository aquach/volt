#pragma once

#include "Game/Core/Core.h"
#include "Game/Entities/Game/Weapon.h"

class Umbrella : public Weapon {
public:
    Umbrella ();
    virtual ~Umbrella ();

    virtual void OnEquip () { }
    virtual void OnUnequip () { }

    virtual void Update ();
    virtual void Render ();

    virtual void OnFire ();
    virtual void OnAlternateFire ();
    virtual void OnAlternateFire2 ();
};
