#pragma once

#include "Core/Core.h"

class Player;

class Weapon {
public:
    Weapon ()
        : m_holder(NULL) { }
    virtual ~Weapon () { }

    virtual void OnFire () { }
    virtual void OnGuard () { }

protected:
    Player* m_holder;

private:
    friend class Player;

    DISALLOW_COPY_AND_ASSIGN(Weapon);
};
