#include "Volt/Game/Entity.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Game/Scene.h"

namespace Volt {

Entity::Entity ()
    : m_layer(0),
      m_scene(NULL),
      m_body(NULL),
      m_visible(true) {
}

Entity::~Entity () {
    if (m_body != NULL) {
        G_PhysicsManager->LockWorld();
        G_PhysicsManager->world()->DestroyBody(m_body);
        G_PhysicsManager->UnlockWorld();
    }
    FOR_(set<EntityContactListener*>::iterator, i,
         m_contactListeners) {
        delete (*i);
    }
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

    G_PhysicsManager->LockWorld();
    m_body = G_PhysicsManager->world()->CreateBody(&def);
    G_PhysicsManager->UnlockWorld();

    return m_body;
}

b2Body* Entity::CreateBody (b2BodyType type) {
    b2BodyDef def;
    def.type = type;
    return CreateBody(def);
}

void Entity::CreateBodyCustom (const string& type, bool dynamic,
                               bool ignoresForces, bool sensor) {
    DestroyBody();

    b2BodyDef def;
    if (ignoresForces)
        def.type = b2_kinematicBody;
    else if (dynamic)
        def.type = b2_dynamicBody;
    else
        def.type = b2_staticBody;

    CreateBody(def);

    b2FixtureDef fixtureDef;
    b2CircleShape circle;
    b2PolygonShape box;
    if (type == "circle") {
        circle.m_radius = m_transform.scale.x * 0.5f;
        fixtureDef.shape = &circle;
    } else if (type == "box") {
        box.SetAsBox(m_transform.scale.x * 0.5f,
                       m_transform.scale.y * 0.5f);
        fixtureDef.shape = &box;
    } else {
        LOG(INFO) << "Warning: invalid type " << type;
        return;
    }

    fixtureDef.isSensor = sensor;
    m_body->CreateFixture(&fixtureDef);
}


void Entity::RemoveSelf () {
    if (m_scene != NULL)
        m_scene->Remove(this);
}

ostream& Entity::ToString (ostream& stream) const {
    stream << "Entity [";
    int size = m_tags.size();
    int count = 0;
    FOR_ (set<string>::const_iterator, i, m_tags) {
        stream << *i << (count == size - 1 ? "" : ", ");
        count++;
    }
    stream << "] at ";
    stream << m_transform;
    return stream;
}

void Entity::SetPosition (Vector2 pos) {
    m_transform.position = pos;
    if (m_body != NULL) {
        G_PhysicsManager->LockWorld();
        m_body->SetTransform(pos.ToB2(), m_transform.rotation * DEG2RAD);
        G_PhysicsManager->UnlockWorld();
    }
}

void Entity::SetRotation (float rotation) {
    m_transform.rotation = rotation;
    if (m_body != NULL) {
        G_PhysicsManager->LockWorld();
        m_body->SetTransform(m_transform.position.ToB2(),
                             m_transform.rotation * DEG2RAD);
        G_PhysicsManager->UnlockWorld();
    }
}

void Entity::DestroyBody () {
    if (m_body != NULL) {
        G_PhysicsManager->LockWorld();
        G_PhysicsManager->world()->DestroyBody(m_body);
        G_PhysicsManager->UnlockWorld();
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
    m_visible = other->m_visible;
}

void Entity::Load (const Json::Value& node) {
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);
    m_visible = node.get("visible", true).asBool();
    SetLayer(node.get("layer", 0).asInt());

    for (uint i = 0; i < node["tags"].size(); i++)
        m_tags.insert(node["tags"][i].asString());
}

void Entity::Save (Json::Value& node) const {
    m_transform.Save(node["transform"]);
    node["visible"] = m_visible;
    node["layer"] = m_layer;
    FOR_(set<string>::iterator, i, m_tags) {
        node["tags"].append(*i);
    }
}

void Entity::AddContactListener (EntityContactListener* listener) {
    m_contactListeners.insert(listener);
}

void Entity::RemoveContactListener (EntityContactListener* listener) {
    m_contactListeners.erase(listener);
}

void Entity::BeginContact (Entity* other, b2Contact* contact) {
    FOR_(set<EntityContactListener*>::iterator, i,
         m_contactListeners) {
        (*i)->OnContactBegin(other, contact);
    }
}

void Entity::EndContact (Entity* other, b2Contact* contact) {
    FOR_(set<EntityContactListener*>::iterator, i,
         m_contactListeners) {
        (*i)->OnContactEnd(other, contact);
    }
}

ostream& operator<< (ostream& stream, const Entity& e) {
   return e.ToString(stream);
}

}
