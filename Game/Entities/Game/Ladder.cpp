#include "Game/Entities/Game/Ladder.h"
#include "Game/Graphics/Graphics.h"

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
    Entity::Load(node);
    CHECK(node["type"].asString() == "Ladder");
    CreatePhysicsBody();
}

void Ladder::Save (Json::Value& node) const {
    Entity::Save(node);
    node["type"] = "Ladder";
}

void Ladder::OnScaleChanged () {
    if (body() != NULL)
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
