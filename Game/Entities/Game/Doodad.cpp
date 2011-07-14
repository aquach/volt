#include "Game/Entities/Game/Doodad.h"
#include "Game/Graphics/Graphics.h"
#include "Game/Editor/Property.h"
#include "Game/Editor/SelectionManager.h"

REGISTER_ENTITY_(Doodad);

Doodad::Doodad () {
    AddTag("Doodad");
    CreatePhysicsBody();

    m_vertexColors[0] = Volt::Color::red;
    m_vertexColors[1] = Volt::Color::green;
    m_vertexColors[2] = Volt::Color::blue;
    m_textureCoords[0].Set(0, 0);
    m_textureCoords[1].Set(1, 0);
    m_textureCoords[2].Set(0, 1);
}

Doodad::~Doodad () {
}

void Doodad::Update () {
}

void Doodad::Render () {
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::BindTexture(m_texture);

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        if (m_texture.HasAsset())
            glTexCoord2f(m_textureCoords[i].x, m_textureCoords[i].y);
        switch (i) {
            case 0: glVertex2f(0, 0); break;
            case 1: glVertex2i(1, 0); break;
            case 2: glVertex2i(1, 1); break;
            case 3: glVertex2i(0, 1); break;
        }
    }
    glEnd();
    glPopMatrix();
}

void Doodad::CreatePhysicsBody () {
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

void Doodad::Load (const Json::Value& node) {
    CHECK(node["type"].asString() == "Doodad");
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);
    CreatePhysicsBody();

    if (node.isMember("textureCoords")) {
        for (int i = 0; i < 4; i++) {
            m_textureCoords[i].Load(node["textureCoords"][i]);
        }
    }
    string path = node.get("texture", "").asString();
    if (path != "")
        m_texture = Volt::G_AssetManager->GetTexture(path);
    if (node.isMember("tint"))
        m_tint.Load(node["tint"]);
}

void Doodad::Save (Json::Value& node) const {
    node["type"] = "Doodad";
    m_transform.Save(node["transform"]);

    for (int i = 0; i < 4; i++) {
        Json::Value v;
        m_textureCoords[i].Save(v);
        node["textureCoords"].append(v);
    }
    node["texture"] = m_texture.HasAsset() ? m_texture->path() : "";
    m_tint.Save(node["tint"]);
}

void Doodad::OnScaleChanged () {
    CreatePhysicsBody();
}

Doodad* Doodad::Clone () const {
    Doodad* newDoodad = new Doodad;
    newDoodad->CopyFrom(this);
    return newDoodad;
}

void Doodad::CopyFrom (const Doodad* other) {
    Entity::CopyFrom(other);
    CreatePhysicsBody();
}

void Doodad::GetProperties (vector<Property*>* properties) {
    Entity::GetProperties(properties);
    //properties->push_back(new StringProperty("Texture", &m_texture));
    properties->push_back(
        new Vector2Property("Vertex 1 Texture Coord", &m_textureCoords[0]));
    properties->push_back(
        new Vector2Property("Vertex 2 Texture Coord", &m_textureCoords[1]));
    properties->push_back(
        new Vector2Property("Vertex 3 Texture Coord", &m_textureCoords[2]));
}


