#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"

class Player;

class Weapon : public Volt::Entity {
public:
    Weapon ()
        : m_holder(NULL) { }
    virtual ~Weapon () { }

    virtual void OnEquip () { }
    virtual void OnUnequip () { }

    virtual void Update () = 0;
    virtual void Render () = 0;

    virtual void OnFire () { }
    virtual void OnAlternateFire () {}
    virtual void OnAlternateFire2 () {}

protected:
    /* TODO: What happens when m_holder is NULL? */
    Player* m_holder;

private:
    friend class Player;

    DISALLOW_COPY_AND_ASSIGN(Weapon);
};
