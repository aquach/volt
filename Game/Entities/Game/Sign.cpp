#include "Game/Entities/Game/Sign.h"
#include "Game/Graphics/Graphics.h"

Sign::Sign () {
    AddTag("Sign");
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
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);
    CreatePhysicsBody();
}

void Sign::Save (Json::Value& node) const {
    m_transform.Save(node["transform"]);
}

void Sign::OnScaleChanged () {
    DestroyBody();
    CreatePhysicsBody();
}

Sign* Sign::Clone () const {
    Sign* newSign = new Sign;
    newSign->CopyFrom(this);
    return newSign;
}

void Sign::CopyFrom (const Sign* other) {
    Entity::CopyFrom(other);
    DestroyBody();
    CreatePhysicsBody();
}

void Sign::OnAccessed (Entity* accessor) {
    // Make text!!
    // TODO: Maybe give Entities a AccessListener interface to register
    // arbitrary access listeners. Also need to make Player trigger Accesses.
}
