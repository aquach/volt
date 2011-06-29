#include "ProjectileController.h"
#include "Volt/Game/Entity.h"

void ProjectileController::Update () {
    Vector2 dir = m_targetPos - m_entity->position();
    float dist = dir.Length();
    float currentSpeed = Vector2(
        m_entity->body()->GetLinearVelocity()).Length();

    float speed = m_def.linearP * (
        dist - m_def.linearD * currentSpeed);
    speed = CLAMP(speed, 0, m_def.maxLinearSpeed);

    Vector2 vel(speed, 0);
    vel = vel.Rotate(m_entity->rotation());
    m_entity->body()->SetLinearVelocity(vel.ToB2());

    float angle = acos(dir.Dot(m_entity->transform().xAxis()) / dist)
                  * Volt::c_rad2deg;
    if (dir.Cross(m_entity->transform().xAxis()) > 0)
        angle *= -1;
    float currentAngularVel = m_entity->body()->GetAngularVelocity()
                              * Volt::c_rad2deg;
    float angularVel = m_def.rotationP * (
        angle - m_def.rotationD * currentAngularVel);
    angularVel = CLAMP(angularVel, -m_def.maxRotationSpeed,
                       m_def.maxRotationSpeed);
    m_entity->body()->SetAngularVelocity(angularVel * Volt::c_deg2rad);
}
