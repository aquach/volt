#include "Game/Entities/Game/Triangle.h"
#include "Game/Graphics/Graphics.h"
#include "Game/Editor/Property.h"
#include "Game/Editor/SelectionManager.h"

REGISTER_ENTITY_(Triangle);

const float SELECT_SLACK = 0.3;

Triangle::Triangle () {
    AddTag("Triangle");
    CreatePhysicsBody();

    m_vertexColors[0] = Volt::Color::red;
    m_vertexColors[1] = Volt::Color::green;
    m_vertexColors[2] = Volt::Color::blue;
    m_textureCoords[0].Set(0, 0);
    m_textureCoords[1].Set(1, 0);
    m_textureCoords[2].Set(0, 1);
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
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    if (m_texture.HasAsset())
        Graphics::BindTexture(m_texture);
    else
        glShadeModel(GL_SMOOTH);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; i++) {
        if (m_texture.HasAsset())
            glTexCoord2f(m_textureCoords[i].x, m_textureCoords[i].y);
        else
            Graphics::SetColor(m_vertexColors[i]);

        switch (i) {
            case 0: glVertex2f(0, 0); break;
            case 1: glVertex2i(1, 0); break;
            case 2: glVertex2i(0, 1); break;
        }
    }
    glEnd();

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
    CHECK(node["type"].asString() == "Triangle");
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);
    CreatePhysicsBody();

    if (node.isMember("vertexColors")) {
        for (int i = 0; i < 3; i++) {
            m_vertexColors[i].Load(node["vertexColors"][i]);
        }
    }
    if (node.isMember("textureCoords")) {
        for (int i = 0; i < 3; i++) {
            m_textureCoords[i].Load(node["textureCoords"][i]);
        }
    }
    string path = node.get("texture", "").asString();
    if (path != "")
        m_texture = Volt::G_AssetManager->GetTexture(path);
}

void Triangle::Save (Json::Value& node) const {
    node["type"] = "Triangle";
    m_transform.Save(node["transform"]);

    for (int i = 0; i < 3; i++) {
        Json::Value v;
        m_vertexColors[i].Save(v);
        node["vertexColors"].append(v);
    }
    for (int i = 0; i < 3; i++) {
        Json::Value v;
        m_textureCoords[i].Save(v);
        node["textureCoords"].append(v);
    }
    node["texture"] = m_texture.HasAsset() ? m_texture->path() : "";
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

void Triangle::GetProperties (vector<Property*>* properties) {
    Entity::GetProperties(properties);

    properties->push_back(
        new ColorProperty("Vertex 1 Color", &m_vertexColors[0]));
    properties->push_back(
        new ColorProperty("Vertex 2 Color", &m_vertexColors[1]));
    properties->push_back(
        new ColorProperty("Vertex 3 Color", &m_vertexColors[2]));
    //properties->push_back(new StringProperty("Texture", &m_texture));
    properties->push_back(
        new Vector2Property("Vertex 1 Texture Coord", &m_textureCoords[0]));
    properties->push_back(
        new Vector2Property("Vertex 2 Texture Coord", &m_textureCoords[1]));
    properties->push_back(
        new Vector2Property("Vertex 3 Texture Coord", &m_textureCoords[2]));
}


