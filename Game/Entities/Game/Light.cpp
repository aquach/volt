#include "Game/Entities/Game/Light.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Game/Scene.h"
#include "Game/Editor/Property.h"
#include "Game/Game/LightManager.h"

REGISTER_ENTITY_(Light);

const char* Light::STATIC_MAP_FOLDER = "generated_lightmaps";
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
      m_listener(NULL),
      m_static(false) {
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

void Light::UpdateNearbyEntities () {
    Vector2 field = Vector2(m_maxDistance, m_maxDistance);
    scene()->GetEntitiesInArea(position() - field,
                               position() + field,
                               &m_nearbyEntities);
}

void Light::Update () {
    if (m_enabled && !m_static) {
        m_nearbyEntitiesTimer -= Volt::G_Time->dt();
        if (m_nearbyEntitiesTimer < 0) {
            m_nearbyEntitiesTimer = REFRESH_TIME;
            UpdateNearbyEntities();
        }
    }
}

void Light::InvalidateStaticMap () {
    m_staticMap = NULL;
    m_nearbyEntities.clear();
    UpdateNearbyEntities();
}

void Light::Render () {
    if (m_enabled)
        G_LightManager->RenderLight(this);

    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::BindTexture(Volt::G_AssetManager->GetTexture("lightstroke.png"));
    glPushMatrix();
    Graphics::Translate(position());
    for (int i = 0; i < m_strokes.size(); i++) {
        LightStroke* stroke = &m_strokes[i];
        Graphics::SetColor(stroke->color);
        Vector2 pos1, pos2;
        pos1.SetFromAngleDegrees(stroke->startAngle);
        pos1 *= stroke->startRadius;
        pos2.SetFromAngleDegrees(stroke->endAngle);
        pos2 *= stroke->endRadius;

        Vector2 perp = (pos2 - pos1).GetPerpendicularRight();
        perp.Normalize();
        perp *= CLAMP((pos2 - pos1).Length() * 0.3f, 0.05, 0.25f);
        Vector2 v1 = pos1 - perp;
        Vector2 v2 = pos1 + perp;
        Vector2 v3 = pos2 - perp;
        Vector2 v4 = pos2 + perp;

        glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex2f(v1.x, v1.y);
        glTexCoord2i(0, 1);
        glVertex2f(v2.x, v2.y);
        glTexCoord2i(1, 1);
        glVertex2f(v4.x, v4.y);
        glTexCoord2i(1, 0);
        glVertex2f(v3.x, v3.y);
        glEnd();
    }
    glPopMatrix();
    Graphics::BindTexture(NULL);
    Graphics::SetBlend(Graphics::BLEND_NONE);
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

    m_name = node["name"].asString();
    m_color.Load(node["color"]);
    m_maxDistance = node["maxDistance"].asDouble();
    m_enabled = node.get("enabled", true).asBool();
    m_static = node.get("static", false).asBool();
    if (m_static) {
        CHECK(node.isMember("staticMap"));
        m_staticMap = Volt::G_AssetManager->GetTexture(node["staticMap"].asString());
    }

    CreatePhysicsBody();
    CreateStrokes();
}

void Light::CreateStrokes () {
    Graphics::BindTexture(m_staticMap);
    unsigned char* image = new unsigned char[
        m_staticMap->width() * m_staticMap->height() * 3];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    Graphics::BindTexture(NULL);

    for (int i = 0; i < 300; i++) {
        float r = Volt::Random::RangeFloat(0.3, 1);
        float dist = r / 4;
        float r2 = r + Volt::Random::RangeFloat(-1, 1) * 0.1;
        r *= m_maxDistance * 0.7f;
        r2 *= m_maxDistance * 0.7f;

        float startAngle = Volt::Random::RangeFloat(0, 360);
        float endAngle = startAngle + Volt::Random::RangeFloat(20, 40);

        bool interrupted = false;
        for (int step = startAngle; step < endAngle; step += 3) {
            Vector2 pos;
            pos.SetFromAngleDegrees(step);
            pos *= r;
            pos /= m_maxDistance;
            int x = (int)((pos.x + 1) / 2 * m_staticMap->width());
            int y = (int)((pos.y + 1) / 2 * m_staticMap->height());
            unsigned char* pixel = image + (y * m_staticMap->width() + x) * 3;
            float intensity = pixel[0] + pixel[1] + pixel[2];
            if (intensity < 0.2f) {
                interrupted = true;
                break;
            }
        }

        if (interrupted) {
            i--;
            continue;
        }

        LightStroke stroke;
        stroke.startRadius = r;
        stroke.startAngle = startAngle;
        stroke.endRadius = r2;
        stroke.endAngle = endAngle;
        stroke.color = (m_color - Volt::Color::Random() * 0.1) * (1 - dist);
        m_strokes.push_back(stroke);
    }
}

void Light::Save (Json::Value& node) const {
    Entity::Save(node);

    node["type"] = "Light";
    m_color.Save(node["color"]);
    node["maxDistance"] = m_maxDistance;
    node["enabled"] = m_enabled;
    node["name"] = m_name;
    node["static"] = m_static;
    if (m_staticMap.HasAsset())
        node["staticMap"] = m_staticMap->path();
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
    properties->push_back(new StringProperty("Name", &m_name));
    properties->push_back(new ColorProperty("Color", &m_color));
    properties->push_back(new FloatProperty("Max Distance", &m_maxDistance));
    properties->push_back(new FloatProperty("Cone Angle", &m_coneAngle));
    properties->push_back(new BoolProperty("Enabled", &m_enabled));
    properties->push_back(new BoolProperty("Static", &m_static));
}

Volt::BBox Light::renderBounds () const {
    Vector2 field(m_maxDistance, m_maxDistance);
    return Volt::BBox(position() - field, position() + field);
}


