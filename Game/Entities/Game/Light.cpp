#include "Game/Entities/Game/Light.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Game/Scene.h"
#include "Game/Editor/Property.h"
#include "Game/Game/LightManager.h"

REGISTER_ENTITY_(Light);

const char* Light::STATIC_MAP_FOLDER = "Textures/generated_lightmaps";
const float REFRESH_TIME = 1.0f;

void LightStroke::Load (const Json::Value& node) {
    CHECK(node.isMember("vertices"));
    CHECK(node["vertices"].size() == 4);

    for (int i = 0; i < 4; i++) {
        vertices[i].Load(node["vertices"][i]);
    }
    CHECK(node.isMember("color"));
    color.Load(node["color"]);
}

void LightStroke::Save (Json::Value& node) const {
    for (int i = 0; i < 4; i++) {
        Json::Value vertNode;
        vertices[i].Save(vertNode);
        node["vertices"].append(vertNode);
    }
    color.Save(node["color"]);
}

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
      m_static(false),
      m_painted(false) {
    m_nearbyEntitiesTimer = Volt::Random::RangeFloat(0.0, 0.1);
    m_listener = new LightSceneListener(this);
    AddTag("Light");
    CreatePhysicsBody();

    m_strokeTexture = G_AssetManager->GetTexture("Textures/lightstroke.png");
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
        m_nearbyEntitiesTimer -= G_Time->dt();
        if (m_nearbyEntitiesTimer < 0) {
            m_nearbyEntitiesTimer = REFRESH_TIME;
            UpdateNearbyEntities();
        }
    }

    if (m_painted && m_strokes.size() == 0)
        GenerateStrokes();
    else if (!m_painted && m_strokes.size() > 0)
        m_strokes.clear();
}

void Light::InvalidateStaticMap () {
    m_staticMap = NULL;
    m_strokes.clear();
    m_nearbyEntities.clear();
    UpdateNearbyEntities();
}

void Light::Render () {
    if (!m_enabled)
        return;

    G_LightManager->RenderLight(this);

    // Render strokes only if painted mode is on and the light is static.
    if (m_painted && m_static) {
        Graphics::SetBlend(Graphics::BLEND_ADDITIVE);
        Graphics::BindTexture(m_strokeTexture);
        glPushMatrix();
        Graphics::Translate(position());
        for (uint i = 0; i < m_strokes.size(); i++) {
            LightStroke* stroke = &m_strokes[i];
            Graphics::SetColor(stroke->color);
            glBegin(GL_QUADS);
            glTexCoord2i(0, 0);
            glVertex2f(stroke->vertices[0].x, stroke->vertices[0].y);
            glTexCoord2i(0, 1);
            glVertex2f(stroke->vertices[1].x, stroke->vertices[1].y);
            glTexCoord2i(1, 1);
            glVertex2f(stroke->vertices[2].x, stroke->vertices[2].y);
            glTexCoord2i(1, 0);
            glVertex2f(stroke->vertices[3].x, stroke->vertices[3].y);
            glEnd();
        }
        glPopMatrix();
        Graphics::BindTexture(NULL);
        Graphics::SetBlend(Graphics::BLEND_NONE);
    }
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
    m_coneAngle = node.get("coneAngle", 360).asInt();
    m_static = node.get("static", false).asBool();
    if (m_static) {
        CHECK(node.isMember("staticMap"));
        m_staticMap = G_AssetManager->GetTexture(node["staticMap"].asString());
    }

    m_painted = node.get("painted", false).asBool();
    if (m_painted) {
        CHECK(node.isMember("strokes"));
        m_strokes.resize(node["strokes"].size());
        for (uint i = 0; i < node["strokes"].size(); i++) {
            m_strokes[i].Load(node["strokes"][i]);
        }
    }

    CreatePhysicsBody();
}

void Light::GenerateStrokes () {
    if (!m_static || !m_staticMap.HasAsset()) {
        // Can't generate strokes for nonstatic lights.
        m_strokes.clear();
        return;
    }

    Graphics::BindTexture(m_staticMap);
    unsigned char* image = new unsigned char[
        m_staticMap->width() * m_staticMap->height() * 3];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    Graphics::BindTexture(NULL);

    // Prevent all-black lights from infinite-looping.
    uint failures = 0;
    int numStrokes = (int)(10.0f * m_maxDistance * m_maxDistance * m_coneAngle
        / 360);
    m_strokes.resize(numStrokes);
    for (uint i = 0; i < m_strokes.size() && failures < m_strokes.size() * 5;
         i++) {

        // Strokes start and end at a pair of (r, theta) coords.
        float startRadius = Volt::Random::RangeFloat(0.5, m_maxDistance * 0.8f);
        float endRadius = startRadius + Volt::Random::RangeFloat(-1, 1) * 0.3;

        // Store dist for falloff color.
        float dist = startRadius / m_maxDistance;

        float startAngle = Volt::Random::RangeFloat(0, 360);

        // Shorten end angle linearly with distance so large distances don't make
        // really stretched out strokes.
        float modifier = m_maxDistance / 5;
        float endAngle = startAngle + Volt::Random::RangeFloat(20, 40) / modifier;

        // Ensure that the arc is actually connected by light.
        bool interrupted = false;
        for (int step = startAngle; step < endAngle; step += 3) {
            Vector2 pos;
            pos.SetFromAngleDegrees(step);
            pos *= startRadius;
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
            failures++;
            i--;
            continue;
        }

        LightStroke& stroke = m_strokes[i];
        Vector2 pos1, pos2;
        pos1.SetFromAngleDegrees(startAngle);
        pos1 *= startRadius;
        pos2.SetFromAngleDegrees(endAngle);
        pos2 *= endRadius;
        float strokeLength = (pos2 - pos1).Length();

        if (strokeLength < 0.4f) {
            failures++;
            i--;
            continue;
        }

        Vector2 perp = (pos2 - pos1).GetPerpendicularRight();
        perp.Normalize();
        perp *= CLAMP(strokeLength * 0.3f, 0.05, 0.25f);
        stroke.vertices[0] = pos1 - perp;
        stroke.vertices[1] = pos1 + perp;
        stroke.vertices[2] = pos2 + perp;
        stroke.vertices[3] = pos2 - perp;
        stroke.color = (m_color - Volt::Color::Random() * 0.2) * (1 - dist);
    }
    LOG(INFO) << "Generated " << m_strokes.size() << " strokes.";

    delete[] image;
}

void Light::Save (Json::Value& node) const {
    Entity::Save(node);

    node["type"] = "Light";
    m_color.Save(node["color"]);
    node["maxDistance"] = m_maxDistance;
    node["coneAngle"] = m_coneAngle;
    node["enabled"] = m_enabled;
    node["name"] = m_name;
    node["static"] = m_static;
    if (m_staticMap.HasAsset())
        node["staticMap"] = m_staticMap->path();
    node["painted"] = m_painted;
    for (uint i = 0; i < m_strokes.size(); i++) {
        Json::Value strokeNode;
        m_strokes[i].Save(strokeNode);
        node["strokes"].append(strokeNode);
    }
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
    m_painted = other->m_painted;
    m_static = other->m_static;
    m_name = other->m_name + "_";
    m_staticMap = other->m_staticMap;
    m_strokes = other->m_strokes;

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
    properties->push_back(new BoolProperty("Painted", &m_painted));
}

Volt::BBox Light::renderBounds () const {
    Vector2 field(m_maxDistance, m_maxDistance);
    return Volt::BBox(position() - field, position() + field);
}


