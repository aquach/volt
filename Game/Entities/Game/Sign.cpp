#include "Game/Entities/Game/Sign.h"
#include "Game/Graphics/Graphics.h"

REGISTER_ENTITY_(Sign);

Sign::Sign () {
    AddTag("Sign");
    CreatePhysicsBody();
}

Sign::~Sign () {
}

void Sign::Render () {
    Graphics::SetColor(Volt::Color::RGB(200, 0, 0));
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderQuad(1, 1);
    glPopMatrix();
}

void Sign::CreatePhysicsBody () {
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

void Sign::Load (const Json::Value& node) {
    Entity::Load(node);
    CHECK(node["type"].asString() == "Sign");
    CreatePhysicsBody();
}

void Sign::Save (Json::Value& node) const {
    Entity::Save(node);
    node["type"] = "Sign";
}

void Sign::OnScaleChanged () {
    if (body() != NULL)
        CreatePhysicsBody();
}

Sign* Sign::Clone () const {
    Sign* newSign = new Sign;
    newSign->CopyFrom(this);
    return newSign;
}

void Sign::CopyFrom (const Sign* other) {
    Entity::CopyFrom(other);
    CreatePhysicsBody();
}
