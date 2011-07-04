#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/Entity.h"

struct Particle {
    Particle ()
        : t(0),
          life(0),
          size(0),
          active(false) {
    }

    float t;
    float life;
    float size;
    Volt::Color color;
    Vector2 position;
    Vector2 velocity;
    bool active;
};

struct ParticleSystemDef {
    ParticleSystemDef ()
        : particlesPerSecond(10),
          color(Volt::Color::red),
          colorVariance(0.4),
          speed(4),
          speedVariance(1),
          life(3),
          lifeVariance(1),
          size(0.1),
          sizeVariance(0.025) {
    }

    float particlesPerSecond;
    Volt::Color color;
    float colorVariance;
    float speed;
    float speedVariance;
    float life;
    float lifeVariance;
    float size;
    float sizeVariance;

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;
};

class ParticleSystem : public Volt::Entity {
public:
    ParticleSystem (Vector2 pos, const ParticleSystemDef& def);
    virtual ~ParticleSystem ();

    virtual void Update ();
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

private:
    list<Particle> m_particles;
    ParticleSystemDef m_def;
    float m_particleTimer;

    DISALLOW_COPY_AND_ASSIGN(ParticleSystem);
};
