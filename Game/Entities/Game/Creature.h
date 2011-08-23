#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"

class Creature;
class Weapon;

/* Subclass this to receive callbacks when the Creature takes damage or dies. */
class CreatureListener {
public:
    CreatureListener () : m_creature(NULL) { }
    virtual ~CreatureListener ();

    virtual void OnDamage (Entity* agent, float damage) { }
    virtual void OnDeath (Entity* agent) { }

    Creature* creature () { return m_creature; }
private:
    friend class Creature;
    Creature* m_creature;
};

class Creature : public Entity {
public:
    Creature ();
    virtual ~Creature ();

    virtual void OnRemoved ();
    virtual void Update () = 0;
    virtual void Render () = 0;

    void EquipWeapon (Weapon* weapon);

    void AddCreatureListener (CreatureListener* listener) {
        CHECK(listener->m_creature == NULL);
        m_listeners.insert(listener);
        listener->m_creature = this;
    }
    void RemoveCreatureListener (CreatureListener* listener) {
        CHECK(listener->m_creature == this);
        m_listeners.erase(listener);
        listener->m_creature = NULL;
    }

    Volt::Transform weaponTransform () const {
        return m_transform.Multiply(m_weaponTransform);
    }

    virtual ostream& ToString (ostream& stream) const;

    void TakeDamage (Entity* agent, float damage);

    int facing () const { return m_facingRight ? 1 : -1; }
    bool facingRight () const { return m_facingRight; }
    void SetFacingRight (bool facing) { m_facingRight = facing; }

protected:
    float m_health;
    float m_maxHealth;
    float m_power;
    float m_maxPower;
    Weapon* m_weapon;
    Volt::Transform m_weaponTransform;
    bool m_facingRight;

private:
    set<CreatureListener*> m_listeners;

    DISALLOW_COPY_AND_ASSIGN(Creature);
};

ostream& operator<< (ostream& stream, const Creature& e);
