#include "Triangle.h"
#include "Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"

Triangle::Triangle () {
}

Triangle::~Triangle () {
}

void Triangle::Update () {
}

void Triangle::Render () {
    Graphics::SetColor(Volt::Color::RGB(0, 0, 100));
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderTriangle(1, 1);
    glPopMatrix();
}

void Triangle::Load (const Json::Value& node) {
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);

    b2BodyDef def;
    def.type = b2_staticBody;
    def.position = b2Vec2(m_transform.position.x, m_transform.position.y);
    def.angle = m_transform.rotation * Volt::c_deg2rad;
    m_body = CreateBody(def);

    b2PolygonShape shape;
    b2Vec2 vertices[3];
    vertices[0] = b2Vec2(0, 0);
    vertices[1] = b2Vec2(m_transform.scale.x, 0);
    vertices[2] = b2Vec2(0, m_transform.scale.y);
    shape.Set(vertices, 3);
    m_body->CreateFixture(&shape, 1);
}

void Triangle::Save (Json::Value& node) const {
    m_transform.Save(node["transform"]);
}
