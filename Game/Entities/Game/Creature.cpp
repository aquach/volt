#include "Game/Entities/Game/Creature.h"
#include "Game/Entities/Game/Weapon.h"

Creature::Creature ()
    : m_health(21),
      m_maxHealth(25),
      m_power(19),
      m_maxPower(20),
      m_weapon(NULL) {
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

void Creature::InvokeHitListeners (Entity* agent, float damage) {
    FOR_(set<CreatureHitListener*>::iterator, i, m_hitListeners) {
        (*i)->OnDamage(agent, damage);
    }
}

ostream& Creature::ToString (ostream& stream) const {
    stream << "Creature ";
    return Entity::ToString(stream);
}

ostream& operator<< (ostream& stream, const Creature& e) {
    return e.ToString(stream);
}
