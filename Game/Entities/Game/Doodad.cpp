#include "Game/Entities/Game/Doodad.h"
#include "Game/Game/DoodadManager.h"
#include "Game/Graphics/Graphics.h"
#include "Game/Editor/Property.h"

REGISTER_ENTITY_(Doodad);

Doodad::Doodad ()
	: m_brush(NULL),
      m_brushId(-1) {
    AddTag("Doodad");
    SetType("Doodad");
}

Doodad::~Doodad () {
}

void Doodad::OnAdded () {
    CreatePhysicsBody();
}

void Doodad::Update () {
}

void Doodad::SetBrush (DoodadBrush* brush) {
    m_brush = brush;
    m_brushId = brush->id;
    m_texture = G_AssetManager->GetTexture(m_brush->texture);
    CreatePhysicsBody();
}

void Doodad::Render () {
	if (m_brush == NULL)
		return;

    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::Scale(m_brush->scale());
    Graphics::Translate(Vector2(-0.5, -0.5));

    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::BindTexture(m_texture);
    Graphics::SetColor(m_tint);

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
		glTexCoord2f(m_brush->textureCoords[i].x, m_brush->textureCoords[i].y);
        switch (i) {
            case 0: glVertex2i(0, 0); break;
            case 1: glVertex2i(1, 0); break;
            case 2: glVertex2i(1, 1); break;
            case 3: glVertex2i(0, 1); break;
        }
    }
    glEnd();
    Graphics::BindTexture(NULL);
    Graphics::SetBlend(Graphics::BLEND_NONE);
    glPopMatrix();

    Graphics::CheckState();
}

void Doodad::CreatePhysicsBody () {
    DestroyBody();
    b2BodyDef def;
    def.type = b2_staticBody;
    m_body = CreateBody(def);

    b2PolygonShape shape;
    float sx = 1;
    float sy = 1;
    if (m_brush != NULL) {
		sx = m_brush->width();
		sy = m_brush->height();
	}
    shape.SetAsBox(sx * m_transform.scale.x * 0.5f,
    			   sy * m_transform.scale.y * 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;
    m_body->CreateFixture(&fixtureDef);
}

void Doodad::Load (const Json::Value& node) {
    Entity::Load(node);
    CHECK(node["type"].asString() == "Doodad");

    if (node.isMember("tint"))
        m_tint.Load(node["tint"]);

    if (node.isMember("brush")) {
        int brushId = node["brush"].asInt();
        SetBrush(G_DoodadManager->GetDoodadBrush(brushId));
    } else {
        CreatePhysicsBody();
    }
}

void Doodad::Save (Json::Value& node) const {
    Entity::Save(node);
    m_tint.Save(node["tint"]);
    if (m_brush != NULL)
        node["brush"] = m_brushId;
}

void Doodad::OnScaleChanged () {
    if (body() != NULL)
        CreatePhysicsBody();
}

Doodad* Doodad::Clone () const {
    Doodad* newDoodad = new Doodad;
    newDoodad->CopyFrom(this);
    return newDoodad;
}

void Doodad::CopyFrom (const Doodad* other) {
    Entity::CopyFrom(other);
    m_tint = other->m_tint;
    SetBrush(G_DoodadManager->GetDoodadBrush(other->m_brushId));
    CreatePhysicsBody();
}

void Doodad::GetProperties (vector<Property*>* properties) {
    Entity::GetProperties(properties);
    //properties->push_back(
    //    new DoodadBrushProperty("Doodad Brush", this));
    properties->push_back(new ColorProperty("Tint", &m_tint));
}


