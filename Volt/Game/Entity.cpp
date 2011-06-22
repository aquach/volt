#include "Entity.h"
#include "PhysicsManager.h"
#include "Scene.h"

namespace Volt {

Entity::Entity ()
    : m_layer(0),
      m_scene(NULL),
      m_parent(NULL),
      m_body(NULL) {
}

Entity::~Entity () {
    if (m_body != NULL)
        G_PhysicsManager->world()->DestroyBody(m_body);
}

void Entity::UpdatePhysics () {
    if (m_body != NULL) {
        b2Vec2 pos = m_body->GetPosition();
        float angle = m_body->GetAngle();
        Vector2 dir = Vector2(pos.x, pos.y) - m_transform.position;
        if (dir.LengthSquared() > 0.01) {
            m_moveDirection = dir;
            m_moveDirection.Normalize();
        }

        m_transform.position.Set(pos.x, pos.y);
        m_transform.rotation = angle * c_rad2deg;
    } else {
        LOG(WARNING) << "No physics body for entity.";
    }
}

b2Body* Entity::CreateBody (b2BodyDef def) {
    def.position.Set(m_transform.position.x, m_transform.position.y);
    def.angle = m_transform.rotation * c_deg2rad;
    def.userData = this;

    m_body = G_PhysicsManager->world()->CreateBody(&def);

    return m_body;
}

b2Body* Entity::CreateBody (b2BodyType type) {
    b2BodyDef def;
    def.type = type;
    return CreateBody(def);
}

void Entity::RemoveSelf () {
    m_scene->Remove(this);
}

}
