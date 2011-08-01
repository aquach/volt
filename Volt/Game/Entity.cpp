#include "Volt/Game/Entity.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Game/Scene.h"

namespace Volt {

Entity::Entity ()
    : m_layer(0),
      m_scene(NULL),
      m_body(NULL) {
}

Entity::~Entity () {
    if (m_body != NULL)
        G_PhysicsManager->world()->DestroyBody(m_body);
}

void Entity::AddTag (const string& tag) {
    pair<set<string>::iterator, bool> result = m_tags.insert(tag);
    if (result.second && m_scene != NULL) {
        m_scene->OnEntityTagAdd(this, tag);
    }
}

void Entity::RemoveTag (const string& tag) {
    int numRemoved = m_tags.erase(tag);
    if (numRemoved > 0 && m_scene != NULL) {
        m_scene->OnEntityTagRemove(this, tag);
    }
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
        m_transform.rotation = angle * RAD2DEG;
    } else {
        LOG(WARNING) << "No physics body for entity.";
    }
}

b2Body* Entity::CreateBody (b2BodyDef def) {
    def.position.Set(m_transform.position.x, m_transform.position.y);
    def.angle = m_transform.rotation * DEG2RAD;
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

ostream& operator<< (ostream& stream, const Entity& e) {
    stream << "Entity [";
    int size = e.m_tags.size();
    int count = 0;
    FOR_ (set<string>::const_iterator, i, e.m_tags) {
        stream << *i << (count == size - 1 ? "" : ", ");
        count++;
    }
    stream << "] at ";
    stream << e.m_transform;
    return stream;
}

void Entity::SetPosition (Vector2 pos) {
    m_transform.position = pos;
    if (m_body != NULL)
        m_body->SetTransform(pos.ToB2(), m_transform.rotation * DEG2RAD);
}

void Entity::SetRotation (float rotation) {
    m_transform.rotation = rotation;
    if (m_body != NULL)
        m_body->SetTransform(m_transform.position.ToB2(),
                             m_transform.rotation * DEG2RAD);
}

void Entity::DestroyBody () {
    if (m_body != NULL) {
        G_PhysicsManager->world()->DestroyBody(m_body);
        m_body = NULL;
    }
}

void Entity::SetScale (Vector2 scale) {
    m_transform.scale = scale;
    OnScaleChanged();
}

void Entity::SetLayer (int layer) {
    int oldLayer = m_layer;
    m_layer = layer;
    if (m_scene == NULL)
        return;
    m_scene->OnEntityLayerChange(this, oldLayer, m_layer);
}

void Entity::CopyFrom (const Entity* other) {
    m_transform = other->m_transform;
}

void Entity::Load (const Json::Value& node) {
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);
}

void Entity::Save (Json::Value& node) const {
    m_transform.Save(node["transform"]);
}

}
