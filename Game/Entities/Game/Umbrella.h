#pragma once

#include "Core/Core.h"
#include "Weapon.h"

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
