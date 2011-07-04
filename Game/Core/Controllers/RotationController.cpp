#include "Game/Core/Controllers/RotationController.h"
#include "Volt/Game/Entity.h"

void RotationController::Update () {
    float angle = m_targetRotation - m_entity->rotation();
    float currentAngularVel = m_entity->body()->GetAngularVelocity()
                              * Volt::RAD2DEG;
    float angularVel = m_p * (angle - m_d * currentAngularVel);
    angularVel = CLAMP(angularVel, -m_maxSpeed, m_maxSpeed);
    m_entity->body()->SetAngularVelocity(angularVel * Volt::DEG2RAD);
}
