#include "Game/Entities/Game/Triangle.h"
#include "Game/Graphics/Graphics.h"
#include "Game/Editor/SelectionManager.h"
#include "Game/Editor/EntityFactory.h"

REGISTER_ENTITY_(Triangle);

const float SELECT_SLACK = 0.3;

Triangle::Triangle () {
    AddTag("Triangle");
    CreatePhysicsBody();
}

Triangle::~Triangle () {
}

void Triangle::Update () {
}

int Triangle::selectedVertex (Vector2 worldPoint) const {
    Vector2 triPoint = m_transform.ApplyInverse(worldPoint);
    if (triPoint.Length() < SELECT_SLACK)
        return 0;
    if ((triPoint - Vector2(1, 0)).Length() < SELECT_SLACK)
        return 1;
    if ((triPoint - Vector2(0, 1)).Length() < SELECT_SLACK)
        return 2;
    return -1;
}

void Triangle::Render () {
    Graphics::SetColor(Volt::Color::RGB(0, 0, 100));
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderTriangle(1, 1);
    if (G_SelectionManager != NULL) {
        // Render selected vertices if necessary.
        if (G_SelectionManager->showVertices()) {
            for (int i = 0; i < 3; i++) {
                if (G_SelectionManager->IsVertexSelected(this, i)) {
                    Graphics::SetColor(Volt::Color::RGB(200, 0, 0));
                } else {
                    Graphics::SetColor(Volt::Color::RGB(50, 50, 250));
                }
                glPointSize(6.0f);
                glBegin(GL_POINTS);
                switch (i) {
                    case 0: glVertex2i(0, 0); break;
                    case 1: glVertex2i(1, 0); break;
                    case 2: glVertex2i(0, 1); break;
                }
                glEnd();
            }
        }
    }
    glPopMatrix();
}

void Triangle::CreatePhysicsBody () {
    DestroyBody();
    b2BodyDef def;
    def.type = b2_staticBody;
    m_body = CreateBody(def);

    b2PolygonShape shape;
    bool flip = m_transform.scale.x * m_transform.scale.y < 0;
    b2Vec2 vertices[3];
    vertices[0] = b2Vec2(0, 0);
    if (flip) {
        vertices[2] = b2Vec2(m_transform.scale.x, 0);
        vertices[1] = b2Vec2(0, m_transform.scale.y);
    } else {
        vertices[1] = b2Vec2(m_transform.scale.x, 0);
        vertices[2] = b2Vec2(0, m_transform.scale.y);
    }
    shape.Set(vertices, 3);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.friction = 1.0f;
    m_body->CreateFixture(&fixtureDef);
}

void Triangle::Load (const Json::Value& node) {
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);
    CreatePhysicsBody();
}

void Triangle::Save (Json::Value& node) const {
    m_transform.Save(node["transform"]);
}

void Triangle::OnScaleChanged () {
    CreatePhysicsBody();
}

Triangle* Triangle::Clone () const {
    Triangle* newTriangle = new Triangle;
    newTriangle->CopyFrom(this);
    return newTriangle;
}

void Triangle::CopyFrom (const Triangle* other) {
    Entity::CopyFrom(other);
    CreatePhysicsBody();
}

