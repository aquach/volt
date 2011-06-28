#include "GUIBar.h"
#include "Volt/Game/Game.h"

void GUIBar::Update () {
    float delta = m_speed * 0.01f * Volt::G_Game->dt();
    if (fabs(m_currentValue - m_target) > delta) {
        m_currentValue += SIGN(m_target - m_currentValue, 1) * delta;
    } else {
        m_currentValue = m_target;
    }
}
