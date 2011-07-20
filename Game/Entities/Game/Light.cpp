#include "Game/Entities/Game/Light.h"
#include "Volt/Game/Game.h"
#include "Volt/Game/Scene.h"
#include "Game/Editor/Property.h"
#include "Game/Game/LightManager.h"

REGISTER_ENTITY_(Light);

const float REFRESH_TIME = 1.0f;

Light::Light ()
	: m_color(Volt::Color::white),
      m_maxDistance(5.0f),
      m_coneAngle(360.0f) {
    m_nearbyEntitiesTimer = Volt::Random::RangeFloat(0.0, 0.1);
    AddTag("Light");
    CreatePhysicsBody();
}

Light::~Light () {
}

void Light::Update () {
    m_nearbyEntitiesTimer -= 0.01; //Volt::G_Game->dt();
    if (m_nearbyEntitiesTimer < 0) {
        m_nearbyEntitiesTimer = REFRESH_TIME;
        Vector2 field = Vector2(m_maxDistance, m_maxDistance);
        scene()->GetEntitiesInArea(position() - field,
                                   position() + field,
                                   &m_nearbyEntities);
    }
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
    CHECK(node.isMember("maxDistance"));

    m_transform.Load(node["transform"]);
    m_color.Load(node["color"]);
    m_maxDistance = node["maxDistance"].asDouble();
    CreatePhysicsBody();
}

void Light::Save (Json::Value& node) const {
    node["type"] = "Light";
    m_transform.Save(node["transform"]);
    m_color.Save(node["color"]);
    node["maxDistance"] = m_maxDistance;
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
    m_maxDistance = other->m_maxDistance;
    m_color = other->m_color;
    m_coneAngle = other->m_coneAngle;
    CreatePhysicsBody();
}

void Light::GetProperties (vector<Property*>* properties) {
    Entity::GetProperties(properties);
    properties->push_back(new ColorProperty("Color", &m_color));
    properties->push_back(new FloatProperty("Max Distance", &m_maxDistance));
    properties->push_back(new FloatProperty("Cone Angle", &m_coneAngle));
}


