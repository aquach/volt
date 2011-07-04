#include "Game/Core/Controllers/PositionController.h"
#include "Volt/Game/Entity.h"

void PositionController::Update () {
    Vector2 dir = m_targetPos - m_entity->position();
    float dist = dir.Length();
    float currentSpeed = Vector2(
        m_entity->body()->GetLinearVelocity()).Length();

    float speed = m_p * (dist - m_d * currentSpeed);
    speed = CLAMP(speed, 0, m_maxSpeed);

    Vector2 vel = dir / dist * speed;
    m_entity->body()->SetLinearVelocity(vel.ToB2());
}
