#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"

class HealthBar;
class PowerBar;
class Weapon;

/* Subclass this to receive callbacks when the Creature takes damage or dies. */
class CreatureListener {
public:
    virtual void OnDamage (Entity* agent, float damage) { };
    virtual void OnDeath (Entity* agent) { };
};

class Creature : public Entity {
public:
    Creature ();
    virtual ~Creature ();

    virtual void OnRemoved ();
    virtual void Update () = 0;
    virtual void Render () = 0;

    void EquipWeapon (Weapon* weapon);

    void AddListener (CreatureListener* listener) {
        m_listeners.insert(listener);
    }
    void RemoveListener (CreatureListener* listener) {
        m_listeners.erase(listener);
    }

    Volt::Transform weaponTransform () const {
        return m_transform.Multiply(m_weaponTransform);
    }

    virtual ostream& ToString (ostream& stream) const;

    void TakeDamage (Entity* agent, float damage);

protected:
    float m_health;
    float m_maxHealth;
    float m_power;
    float m_maxPower;
    Weapon* m_weapon;
    Volt::Transform m_weaponTransform;

private:
    set<CreatureListener*> m_listeners;

    DISALLOW_COPY_AND_ASSIGN(Creature);
};

ostream& operator<< (ostream& stream, const Creature& e);
