#include "RotationController.h"
#include "Volt/Game/Entity.h"

void RotationController::Update () {
    float angle = m_targetRotation - m_entity->rotation();
    float currentAngularVel = m_entity->body()->GetAngularVelocity()
                              * Volt::c_rad2deg;
    float angularVel = m_p * (angle - m_d * currentAngularVel);
    angularVel = CLAMP(angularVel, -m_maxSpeed, m_maxSpeed);
    m_entity->body()->SetAngularVelocity(angularVel * Volt::c_deg2rad);
}
