#include "Game/Entities/Game/Doodad.h"
#include "Game/Game/DoodadManager.h"
#include "Game/Graphics/Graphics.h"
#include "Game/Editor/Property.h"
#include "Game/Editor/SelectionManager.h"

REGISTER_ENTITY_(Doodad);

Doodad::Doodad ()
	: m_brush(NULL) {
    AddTag("Doodad");
    CreatePhysicsBody();
}

Doodad::~Doodad () {
}

void Doodad::Update () {
}

void Doodad::Render () {
	if (m_brush == NULL)
		return;

    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::BindTexture(m_brush->texture);

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
		glTexCoord2f(m_brush->textureCoords[i].x, m_brush->textureCoords[i].y);
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
    float sx = 1;
    float sy = 1;
    if (m_brush != NULL) {
		sx = m_brush->size.x;
		sy = m_brush->size.y;
	}
    shape.SetAsBox(sx * m_transform.scale.x * 0.5f,
    			   sy * m_transform.scale.y * 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;
    m_body->CreateFixture(&fixtureDef);
}

void Doodad::Load (const Json::Value& node) {
    CHECK(node["type"].asString() == "Doodad");
    CHECK(node.isMember("transform"));
    m_transform.Load(node["transform"]);
    CreatePhysicsBody();
    if (node.isMember("tint"))
        m_tint.Load(node["tint"]);
}

void Doodad::Save (Json::Value& node) const {
    node["type"] = "Doodad";
    m_transform.Save(node["transform"]);
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
    m_tint = other->m_tint;
    CreatePhysicsBody();
}

void Doodad::GetProperties (vector<Property*>* properties) {
    Entity::GetProperties(properties);
    //properties->push_back(
    //    new DoodadBrushProperty("Doodad Brush", this));
}


