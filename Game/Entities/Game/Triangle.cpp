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
    m_body = CreateBody(def);

    b2PolygonShape shape;
    b2Vec2 vertices[3];
    Vector2 v1 = m_transform.Apply(Vector2(0, 0));
    Vector2 v2 = m_transform.Apply(Vector2(1, 0));
    Vector2 v3 = m_transform.Apply(Vector2(0, 1));
    vertices[0] = b2Vec2(v1.x, v1.y);
    vertices[1] = b2Vec2(v2.x, v2.y);
    vertices[2] = b2Vec2(v3.x, v3.y);
    shape.Set(vertices, 3);
    m_body->CreateFixture(&shape, 1);
}

void Triangle::Save (Json::Value& node) const {
    m_transform.Save(node["transform"]);
}