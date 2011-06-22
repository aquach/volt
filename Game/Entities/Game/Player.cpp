#include "Player.h"
#include "Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"

Player::Player () {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.fixedRotation = true;
    m_body = CreateBody(def);

    b2PolygonShape shape;
    shape.SetAsBox(1, 1);
    m_body->CreateFixture(&shape, 1);
}

Player::~Player () {
}

void Player::Update () {
    UpdatePhysics();
}

void Player::Render () {
    Graphics::SetBlend(Graphics::BLEND_NONE);
    glLineWidth(2.0f);
    glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderQuad(2, 2);
    glPopMatrix();
}
