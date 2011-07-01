#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"

class Creature;

/* Subclass this to receive callbacks when the weapon strikes a Creature. */
class WeaponStrikeListener {
public:
    virtual void OnStrike (Creature* target, float damage) = 0;
};

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

    Creature* holder () const { return m_holder; }

    void AddStrikeListener (WeaponStrikeListener* listener) {
        m_strikeListeners.insert(listener);
    }
    void RemoveStrikeListener (WeaponStrikeListener* listener) {
        m_strikeListeners.erase(listener);
    }

protected:
    /* TODO: What happens when m_holder is NULL? */
    Creature* m_holder;

private:
    friend class Creature;

    void InvokeStrikeListeners (Creature* target, float damage);
    set<WeaponStrikeListener*> m_strikeListeners;

    DISALLOW_COPY_AND_ASSIGN(Weapon);
};
