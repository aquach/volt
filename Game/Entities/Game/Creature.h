#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/Entity.h"

class HealthBar;
class PowerBar;
class Weapon;

/* Subclass this to receive callbacks when the Creature takes damage. */
class CreatureHitListener {
public:
    virtual void OnHit (Volt::Entity* agent, float damage) = 0;
};

class Creature : public Volt::Entity {
public:
    Creature ();
    virtual ~Creature () { }

    virtual void Update () = 0;
    virtual void Render () = 0;

    // TODO: Solve dangling weapons when creature removed?
    void EquipWeapon (Weapon* weapon);

    void AddHitListener (CreatureHitListener* listener) {
        m_hitListeners.insert(listener);
    }
    void RemoveHitListener (CreatureHitListener* listener) {
        m_hitListeners.erase(listener);
    }

    Volt::Transform weaponTransform () const {
        return m_transform.Multiply(m_weaponTransform);
    }

protected:
    void InvokeHitListeners (Volt::Entity* agent, float damage);
    float m_health;
    float m_maxHealth;
    float m_power;
    float m_maxPower;
    Weapon* m_weapon;
    Volt::Transform m_weaponTransform;

private:
    set<CreatureHitListener*> m_hitListeners;

    DISALLOW_COPY_AND_ASSIGN(Creature);
};
