#include "Game/Entities/Game/Light.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Game/Scene.h"
#include "Game/Editor/Property.h"
#include "Game/Game/LightManager.h"

REGISTER_ENTITY_(Light);

const float REFRESH_TIME = 1.0f;

void Light::LightSceneListener::OnEntityRemoved (Volt::Entity* e) {
    for (vector<Volt::Entity*>::iterator i = m_light->m_nearbyEntities.begin();
         i != m_light->m_nearbyEntities.end();) {
        if (*i == e)
            i = m_light->m_nearbyEntities.erase(i);
        else
            i++;
    }
};

Light::Light ()
	: m_color(Volt::Color::white),
      m_maxDistance(5.0f),
      m_coneAngle(360.0f),
      m_enabled(true),
      m_listener(NULL) {
    m_nearbyEntitiesTimer = Volt::Random::RangeFloat(0.0, 0.1);
    m_listener = new LightSceneListener(this);
    AddTag("Light");
    CreatePhysicsBody();
}

Light::~Light () {
    delete m_listener;
}

void Light::OnAdded () {
    scene()->AddSceneListener(m_listener);
}

void Light::OnRemoved () {
    scene()->RemoveSceneListener(m_listener);
}

void Light::Update () {
    if (m_enabled) {
        m_nearbyEntitiesTimer -= Volt::G_Time->dt();
        if (m_nearbyEntitiesTimer < 0) {
            m_nearbyEntitiesTimer = REFRESH_TIME;
            Vector2 field = Vector2(m_maxDistance, m_maxDistance);
            scene()->GetEntitiesInArea(position() - field,
                                       position() + field,
                                       &m_nearbyEntities);
        }
    }
}

void Light::Render () {
    if (m_enabled)
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
    Entity::Load(node);
    CHECK(node["type"].asString() == "Light");
    CHECK(node.isMember("color"));
    CHECK(node.isMember("maxDistance"));

    m_color.Load(node["color"]);
    m_maxDistance = node["maxDistance"].asDouble();
    m_enabled = node.get("enabled", true).asBool();
    CreatePhysicsBody();
}

void Light::Save (Json::Value& node) const {
    Entity::Save(node);

    node["type"] = "Light";
    m_color.Save(node["color"]);
    node["maxDistance"] = m_maxDistance;
    node["enabled"] = m_enabled;
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
    m_enabled = other->m_enabled;
    CreatePhysicsBody();
}

void Light::GetProperties (vector<Property*>* properties) {
    Entity::GetProperties(properties);
    properties->push_back(new ColorProperty("Color", &m_color));
    properties->push_back(new FloatProperty("Max Distance", &m_maxDistance));
    properties->push_back(new FloatProperty("Cone Angle", &m_coneAngle));
    //properties->push_back(new BoolProperty("Enabled", &m_enabled));
}


