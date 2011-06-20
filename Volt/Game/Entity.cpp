#include "Entity.h"

namespace Volt {

Entity::Entity ()
    : m_layer(0),
      m_scene(NULL),
      m_parent(NULL),
      m_body(NULL) {
}

void Entity::UpdatePhysics () {
    if (m_body != NULL) {
        b2Vec2 pos = m_body->GetPosition();
        float angle = m_body->GetAngle();
        m_transform.position.Set(pos.x, pos.y);
        m_transform.rotation = angle * c_rad2deg;
    }
}

}
