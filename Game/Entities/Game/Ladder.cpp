#include "Game/Entities/Game/Ladder.h"
#include "Game/Graphics/Graphics.h"
#include "Game/Editor/EntityFactory.h"

REGISTER_ENTITY_(Ladder);

Ladder::Ladder () {
    AddTag("Ladder");
    CreatePhysicsBody();
}

Ladder::~Ladder () {
}

void Ladder::Render () {
    Graphics::SetColor(Volt::Color::RGB(0, 100, 0));
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderQuad(1, 1);
    glPopMatrix();
}

void Ladder::CreatePhysicsBody () {
    DestroyBody();
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

void Ladder::Load (const Json::Value& node) {
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);
    CreatePhysicsBody();
}

void Ladder::Save (Json::Value& node) const {
    m_transform.Save(node["transform"]);
}

void Ladder::OnScaleChanged () {
    CreatePhysicsBody();
}

Ladder* Ladder::Clone () const {
    Ladder* newLadder = new Ladder;
    newLadder->CopyFrom(this);
    return newLadder;
}

void Ladder::CopyFrom (const Ladder* other) {
    Entity::CopyFrom(other);
    CreatePhysicsBody();
}
