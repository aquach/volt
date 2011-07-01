#include "VelocityController.h"
#include "Volt/Game/Entity.h"

void VelocityController::Update () {
    Vector2 dir = m_targetVel - m_entity->body()->GetLinearVelocity();
    float len = dir.Length();
    if (len > 0.1) {
        Vector2 force;
        force.x = dir.x * m_impulse.x;
        force.y = dir.y * m_impulse.y;
        m_entity->body()->ApplyLinearImpulse(force.ToB2(),
            m_entity->body()->GetWorldCenter());
    }
}
