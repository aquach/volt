#include "Game/Entities/Game/Weapon.h"

void Weapon::InvokeStrikeListeners (Creature* target, float damage) {
    FOR_(set<WeaponStrikeListener*>::iterator, i, m_strikeListeners) {
        (*i)->OnStrike(target, damage);
    }
}
