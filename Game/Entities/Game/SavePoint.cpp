#include "Game/Entities/Game/SavePoint.h"
#include "Game/Graphics/Graphics.h"
#include "Game/Editor/EntityFactory.h"

REGISTER_ENTITY_(SavePoint);

SavePoint::SavePoint () {
    AddTag("SavePoint");
}

SavePoint::~SavePoint () {
}

void SavePoint::Render () {
    Graphics::SetColor(Volt::Color::RGB(150, 0, 150));
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderQuad(1, 1);
    glPopMatrix();
}

void SavePoint::CreatePhysicsBody () {
    b2BodyDef def;
    def.type = b2_staticBody;
    m_body = CreateBody(def);

    b2PolygonShape shape;
    shape.SetAsBox(m_transform.scale.x * 0.5f, m_transform.scale.y * 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;
    m_body->CreateFixture(&fixtureDef);
}

void SavePoint::Load (const Json::Value& node) {
    CHECK(node["type"].asString() == "SavePoint");
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);
    CreatePhysicsBody();
}

void SavePoint::Save (Json::Value& node) const {
    node["type"] = "SavePoint";
    m_transform.Save(node["transform"]);
}

void SavePoint::OnScaleChanged () {
    DestroyBody();
    CreatePhysicsBody();
}

SavePoint* SavePoint::Clone () const {
    SavePoint* newSavePoint = new SavePoint;
    newSavePoint->CopyFrom(this);
    return newSavePoint;
}

void SavePoint::CopyFrom (const SavePoint* other) {
    Entity::CopyFrom(other);
    DestroyBody();
    CreatePhysicsBody();
}

void SavePoint::OnAccessed (Entity* accessor) {
    // TODO: Save dialog.
}
