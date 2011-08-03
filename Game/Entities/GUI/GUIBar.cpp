#include "Game/Entities/GUI/GUIBar.h"
#include "Volt/Game/AppTime.h"

void GUIBar::Update () {
    float delta = m_speed * 0.01f * G_Time->dt();
    m_currentValue = APPROACH(m_currentValue, m_target,
                              SIGNOF(m_target - m_currentValue) * delta);
}
