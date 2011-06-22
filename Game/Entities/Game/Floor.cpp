#include "Floor.h"
#include "Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"

Floor::Floor () {
    m_transform.position.y = 4;
    m_transform.rotation = 50;

    m_body = CreateBody(b2_staticBody);

    b2PolygonShape shape;
    shape.SetAsBox(1.5, 0.1);
    m_body->CreateFixture(&shape, 1);
}

Floor::~Floor () {
}

void Floor::Update () {
}

void Floor::Render () {
    Graphics::SetBlend(Graphics::BLEND_NONE);
    glLineWidth(2.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderQuad(3, 0.2);
    glPopMatrix();
}
