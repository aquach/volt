#include "Game/Entities/Game/Creature.h"
#include "Game/Entities/Game/Weapon.h"

Creature::Creature ()
    : m_health(21),
      m_maxHealth(25),
      m_power(19),
      m_maxPower(20),
      m_weapon(NULL) {
}

void Creature::EquipWeapon (Weapon* weapon) {
    if (m_weapon != NULL) {
        m_weapon->OnUnequip();
        m_weapon->m_holder = NULL;
    }
    m_weapon = weapon;
    m_weapon->m_holder = this;
    if (m_weapon != NULL)
        m_weapon->OnEquip();
}

void Creature::InvokeHitListeners (Volt::Entity* agent, float damage) {
    FOR_(set<CreatureHitListener*>::iterator, i, m_hitListeners) {
        (*i)->OnHit(agent, damage);
    }
}
