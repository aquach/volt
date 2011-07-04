#include "Game/Core/Controllers/ProjectileController.h"
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

    float angle = dir.GetAngleDegrees() - m_entity->rotation();
    float currentAngularVel = m_entity->body()->GetAngularVelocity()
                              * Volt::RAD2DEG;
    float angularVel = m_def.rotationP * (
        angle - m_def.rotationD * currentAngularVel);
    angularVel = CLAMP(angularVel, -m_def.maxRotationSpeed,
                       m_def.maxRotationSpeed);
    m_entity->body()->SetAngularVelocity(angularVel * Volt::DEG2RAD);
}
