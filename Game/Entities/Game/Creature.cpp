#include "Game/Entities/Game/Creature.h"
#include "Game/Entities/Game/Weapon.h"

CreatureListener::~CreatureListener () {
    if (m_creature != NULL)
        m_creature->RemoveCreatureListener(this);
}

Creature::Creature ()
    : m_health(21),
      m_maxHealth(25),
      m_power(19),
      m_maxPower(20),
      m_weapon(NULL) {
}

Creature::~Creature () {
    FOR_(set<CreatureListener*>::iterator, i, m_listeners) {
        delete *i;
    }
    m_listeners.clear();
}

void Creature::OnRemoved () {
    if (m_weapon != NULL) {
        m_weapon->OnUnequip();
        m_weapon->RemoveSelf();
    }
}

void Creature::EquipWeapon (Weapon* weapon) {
    if (m_weapon != NULL) {
        m_weapon->OnUnequip();
        m_weapon->m_holder = NULL;
    }
    m_weapon = weapon;
    if (m_weapon != NULL) {
        m_weapon->m_holder = this;
        m_weapon->OnEquip();
    }
}

void Creature::TakeDamage (Entity* agent, float damage) {
    if (m_health <= 0)
        return;

    m_health -= damage;
    FOR_(set<CreatureListener*>::iterator, i, m_listeners) {
        (*i)->OnDamage(agent, damage);
    }
    if (m_health <= 0) {
        // Invoke kill listener.
        FOR_(set<CreatureListener*>::iterator, i, m_listeners) {
            (*i)->OnDeath(agent);
        }
    }
}

ostream& Creature::ToString (ostream& stream) const {
    stream << "Creature ";
    return Entity::ToString(stream);
}

ostream& operator<< (ostream& stream, const Creature& e) {
    return e.ToString(stream);
}
