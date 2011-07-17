#include "Game/Entities/Game/Light.h"
#include "Game/Editor/Property.h"
#include "Game/Game/LightManager.h"

REGISTER_ENTITY_(Light);

Light::Light ()
	: m_color(Volt::Color::white),
      m_intensity(1.0f) {
    AddTag("Light");
    CreatePhysicsBody();
}

Light::~Light () {
}

void Light::Update () {
    return;
}

void Light::Render () {
    G_LightManager->RenderLight(this);
}

void Light::CreatePhysicsBody () {
    DestroyBody();
    b2BodyDef def;
    def.type = b2_staticBody;
    m_body = CreateBody(def);

    b2CircleShape shape;
    shape.m_radius = 0.5;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;
    m_body->CreateFixture(&fixtureDef);
}

void Light::Load (const Json::Value& node) {
    CHECK(node["type"].asString() == "Light");
    CHECK(node.isMember("transform"));
    CHECK(node.isMember("color"));
    CHECK(node.isMember("intensity"));

    m_transform.Load(node["transform"]);
    m_color.Load(node["color"]);
    m_intensity = node["intensity"].asDouble();
    CreatePhysicsBody();
}

void Light::Save (Json::Value& node) const {
    node["type"] = "Light";
    m_transform.Save(node["transform"]);
    m_color.Save(node["color"]);
    node["intensity"] = m_intensity;
}

void Light::OnScaleChanged () {
    CreatePhysicsBody();
}

Light* Light::Clone () const {
    Light* newLight = new Light;
    newLight->CopyFrom(this);
    return newLight;
}

void Light::CopyFrom (const Light* other) {
    Entity::CopyFrom(other);
    m_intensity = other->m_intensity;
    m_color = other->m_color;
    CreatePhysicsBody();
}

void Light::GetProperties (vector<Property*>* properties) {
    Entity::GetProperties(properties);
    properties->push_back(new ColorProperty("Color", &m_color));
    properties->push_back(new FloatProperty("Intensity", &m_intensity));
}


