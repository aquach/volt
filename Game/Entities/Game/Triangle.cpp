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

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < NUM_VERTS; i++) {
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
    Graphics::BindTexture(NULL);

    if (G_SelectionManager != NULL) {
        // Render selected vertices if necessary.
        if (G_SelectionManager->showVertices()) {
            glPointSize(6.0f);
            for (int i = 0; i < NUM_VERTS; i++) {
                if (G_SelectionManager->IsVertexSelected(this, i)) {
                    Graphics::SetColor(Volt::Color::RGB(200, 0, 0));
                } else {
                    Graphics::SetColor(Volt::Color::RGB(50, 50, 250));
                }
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
    glPushMatrix();
    Graphics::Translate(m_transform.position);
    glRotatef(m_transform.rotation, 0.0f, 0.0f, 1.0f);
    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::BindTexture(Volt::G_AssetManager->GetTexture("brush.png"));
    for (int i = 0; i < m_strokes.size(); i++) {
        glPushMatrix();
        Graphics::Translate(Vector2(m_strokes[i].transform.position.x
            * m_transform.scale.x, m_strokes[i].transform.position.y
            * m_transform.scale.y));
        glRotatef(m_strokes[i].transform.rotation, 0.0f, 0.0f, 1.0f);
        Graphics::Scale(m_strokes[i].transform.scale);
        Graphics::SetColor(m_strokes[i].color);
        Graphics::RenderQuad(1, 1);
        glPopMatrix();
    }
    Graphics::SetBlend(Graphics::BLEND_NONE);
    Graphics::BindTexture(NULL);
    glPopMatrix();
}

void Triangle::CreatePhysicsBody () {
    DestroyBody();
    b2BodyDef def;
    def.type = b2_staticBody;
    m_body = CreateBody(def);

    b2PolygonShape shape;
    bool flip = m_transform.scale.x * m_transform.scale.y < 0;
    b2Vec2 vertices[NUM_VERTS];
    vertices[0] = b2Vec2(0, 0);
    if (flip) {
        vertices[2] = b2Vec2(m_transform.scale.x, 0);
        vertices[1] = b2Vec2(0, m_transform.scale.y);
    } else {
        vertices[1] = b2Vec2(m_transform.scale.x, 0);
        vertices[2] = b2Vec2(0, m_transform.scale.y);
    }
    shape.Set(vertices, NUM_VERTS);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.friction = 1.0f;
    m_body->CreateFixture(&fixtureDef);
}

void Triangle::Load (const Json::Value& node) {
    Entity::Load(node);
    CHECK(node["type"].asString() == "Triangle");
    CreatePhysicsBody();

    if (node.isMember("vertexColors")) {
        for (int i = 0; i < NUM_VERTS; i++) {
            m_vertexColors[i].Load(node["vertexColors"][i]);
        }
    }
    if (node.isMember("textureCoords")) {
        for (int i = 0; i < NUM_VERTS; i++) {
            m_textureCoords[i].Load(node["textureCoords"][i]);
        }
    }
    string path = node.get("texture", "").asString();
    if (path != "")
        m_texture = Volt::G_AssetManager->GetTexture(path);

    GenerateStrokes();
}

Volt::Color ColorAtPoint (Vector2 pos) {
    /*
     * x1 = 0 y1 = 0
     * x2 = 1 y2 = 0
     * x3 = 0 y3 = 1
     */
    float a = -pos.x - pos.y + 1;
    float b = pos.x;
    float c = 1 - a - b;

    if (a < 0 || b < 0 || c < 0)
        return Volt::Color::black;
    if (a > 1 || b > 1 || c > 1)
        return Volt::Color::black;

    Volt::Color color = Volt::Color::red * a + Volt::Color::green * b
        + Volt::Color::blue * c;

    return color;
}

Vector2 ColorGradientAtPoint (Vector2 pos) {
    Volt::Color dx = ColorAtPoint(pos + Vector2(0.1, 0)) - ColorAtPoint(pos - Vector2(0.1, 0));
    Volt::Color dy = ColorAtPoint(pos + Vector2(0, 0.1)) - ColorAtPoint(pos - Vector2(0, 0.1));
    Vector2 gradient(dx.Intensity() / 2, dy.Intensity() / 2);
    return gradient;
}

void Triangle::GenerateStrokes () {
    m_strokes.clear();
    int numStrokes = 50 * (m_transform.scale.x + m_transform.scale.y) / 2;
    for (int i = 0; i < numStrokes; i++) {
        Vector2 pos(Volt::Random::Percent(), Volt::Random::Percent());
        Volt::Color color = ColorAtPoint(pos);
        if (color.Intensity() == 0) {
            i--;
            continue;
        }

        Vector2 gradient = ColorGradientAtPoint(pos).GetPerpendicularRight();

        BrushStroke stroke;
        stroke.transform.position = pos;
        stroke.transform.rotation = gradient.GetAngleDegrees();
        stroke.transform.scale.Set(0.5, 0.5);
        stroke.color = color + Volt::Color::Random() * 0.2;
        m_strokes.push_back(stroke);
    }
}

void Triangle::Save (Json::Value& node) const {
    Entity::Save(node);
    node["type"] = "Triangle";

    for (int i = 0; i < NUM_VERTS; i++) {
        Json::Value v;
        m_vertexColors[i].Save(v);
        node["vertexColors"].append(v);
    }
    for (int i = 0; i < NUM_VERTS; i++) {
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
    m_texture = other->m_texture;
    copy(other->m_textureCoords, other->m_textureCoords + NUM_VERTS, m_textureCoords);
    copy(other->m_vertexColors, other->m_vertexColors + NUM_VERTS, m_vertexColors);
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


