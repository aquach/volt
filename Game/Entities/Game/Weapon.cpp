#include "Game/Entities/Game/Weapon.h"
#include "Game/Entities/Game/Creature.h"

void Weapon::InvokeStrikeListeners (Creature* target, float damage) {
    FOR_(set<WeaponStrikeListener*>::iterator, i, m_strikeListeners) {
        (*i)->OnStrike(target, damage);
    }
}

void Weapon::OnRemoved () {
    if (m_holder != NULL)
        m_holder->EquipWeapon(NULL);
}
