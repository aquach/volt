#include "Game/Entities/Graphics/ParticleSystem.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Game/Scene.h"
#include "Game/Editor/Property.h"
#include "Game/Graphics/Graphics.h"

REGISTER_ENTITY_(ParticleSystem);

void ParticleSystemDef::Load (const Json::Value& node) {
    CHECK(node.isMember("particlesPerSecond"));
    CHECK(node.isMember("color"));
    CHECK(node.isMember("colorVariance"));
    CHECK(node.isMember("speed"));
    CHECK(node.isMember("speedVariance"));
    CHECK(node.isMember("life"));
    CHECK(node.isMember("lifeVariance"));
    CHECK(node.isMember("size"));
    CHECK(node.isMember("sizeVariance"));

    particlesPerSecond = node["particlesPerSecond"].asDouble();
    color.Load(node["color"]);
    colorVariance = node["colorVariance"].asDouble();
    speed = node["speed"].asDouble();
    speedVariance = node["speedVariance"].asDouble();
    life = node["life"].asDouble();
    lifeVariance = node["lifeVariance"].asDouble();
    size = node["size"].asDouble();
    sizeVariance = node["sizeVariance"].asDouble();
}

void ParticleSystemDef::Save (Json::Value& node) const {
    node["particlesPerSecond"] = particlesPerSecond;
    color.Save(node["color"]);
    node["colorVariance"] = colorVariance;
    node["speed"] = speed;
    node["speedVariance"] = speedVariance;
    node["life"] = life;
    node["lifeVariance"] = lifeVariance;
    node["size"] = size;
    node["sizeVariance"] = sizeVariance;
}

ParticleSystem::ParticleSystem ()
    : m_particleTimer(0) {
    ResizeParticleArray();
}

ParticleSystem::ParticleSystem (Vector2 pos, const ParticleSystemDef& def)
    : m_def(def),
      m_particleTimer(0) {
    m_transform.position = pos;
    ResizeParticleArray();
}

void ParticleSystem::ResizeParticleArray () {
    // Make room for enough particles to not have to make more room.
    int room = (int)((m_def.life + m_def.lifeVariance)
                * m_def.particlesPerSecond * 1.1);
    m_particles.resize(room);
}

ParticleSystem::~ParticleSystem () {
}

void ParticleSystem::Update () {
    m_particleTimer += Volt::G_Time->dt();
    int numNewParticles = 0;
    float secondsPerParticle = 1 / m_def.particlesPerSecond;
    while (m_particleTimer > secondsPerParticle) {
        numNewParticles++;
        m_particleTimer -= secondsPerParticle;
    }

    for (list<Particle>::iterator i = m_particles.begin();
         i != m_particles.end(); i++) {
        Particle* p = &(*i);
        if (p->active) {
            p->t += Volt::G_Time->dt();
            if (p->t > p->life)
                p->active = false;
            else
                p->position += p->velocity * Volt::G_Time->dt();
        } else if (numNewParticles > 0) {
            // Spawn new particle.
            p->t = 0;
            p->life = m_def.life +
                Volt::Random::RangeFloat(-m_def.lifeVariance,
                                         m_def.lifeVariance);
            p->size = m_def.size +
                Volt::Random::RangeFloat(-m_def.sizeVariance,
                                         m_def.sizeVariance);
            p->velocity = Vector2::Random() * (m_def.speed +
                Volt::Random::RangeFloat(-m_def.speedVariance,
                                         m_def.speedVariance));
            p->position = position();
            p->color = m_def.color;
            p->color.r += Volt::Random::RangeFloat(-m_def.colorVariance,
                                                   m_def.colorVariance);
            p->color.g += Volt::Random::RangeFloat(-m_def.colorVariance,
                                                   m_def.colorVariance);
            p->color.b += Volt::Random::RangeFloat(-m_def.colorVariance,
                                                   m_def.colorVariance);
            p->color.Clamp();
            p->active = true;
            numNewParticles--;
        }
    }
    CHECK(numNewParticles == 0) << "Did not have enough room to make "
                                << numNewParticles << " particles, list of "
                                << "size " << m_particles.size();
}

void ParticleSystem::Render () {
    FOR_ (list<Particle>::iterator, i, m_particles) {
        Particle* p = &(*i);
        if (!p->active)
            continue;

        glPointSize(p->size * scene()->camera()->transform.scale.Length());
        Graphics::SetColor(p->color);
        glBegin(GL_POINTS);
        glVertex2f(p->position.x, p->position.y);
        glEnd();
    }
}

void ParticleSystem::Load (const Json::Value& node) {
    Entity::Load(node);
    CHECK(node.isMember("def"));
    m_def.Load(node["def"]);
}

void ParticleSystem::Save (Json::Value& node) const {
    Entity::Save(node);
    m_def.Save(node["def"]);
}

void ParticleSystem::GetProperties (vector<Property*>* properties) {
    properties->push_back(new FloatProperty("Particles / Second",
                                            &m_def.particlesPerSecond));
    properties->push_back(new ColorProperty("Color", &m_def.color));
    properties->push_back(new FloatProperty("Color Variance",
                                            &m_def.colorVariance));
    properties->push_back(new FloatProperty("Speed", &m_def.speed));
    properties->push_back(new FloatProperty("Speed Variance",
                                            &m_def.speedVariance));
    properties->push_back(new FloatProperty("Life", &m_def.life));
    properties->push_back(new FloatProperty("Life Variance",
                                            &m_def.lifeVariance));
    properties->push_back(new FloatProperty("Size", &m_def.size));
    properties->push_back(new FloatProperty("Size Variance",
                                            &m_def.sizeVariance));
}
