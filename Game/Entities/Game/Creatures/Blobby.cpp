#include "Blobby.h"
#include "Core/Controllers/VelocityController.h"
#include "Graphics/Graphics.h"
#include "Volt/Game/Game.h"

static const float RADIUS = 0.8;

Blobby::Blobby ()
    : m_moveTimer(0),
      m_pauseTimer(0) {

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.fixedRotation = true;
    m_body = CreateBody(def);

    b2CircleShape shape;
    shape.m_radius = RADIUS;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.friction = 0.5f;
    m_body->CreateFixture(&fixtureDef);

    m_controller = new VelocityController(this, Vector2::zero);
    m_controller->SetImpulse(Vector2(1, 0));
}

void Blobby::Update () {
    UpdatePhysics();

    m_moveTimer -= Volt::G_Game->dt();
    m_pauseTimer -= Volt::G_Game->dt();

    m_controller->Update();

    if (m_pauseTimer <= 0 && m_moveTimer <= 0) {
        m_moveTimer = 4;
        m_controller->SetTargetVelocity(Vector2(
            5 * (Volt::Random::CoinFlip() ? 1 : -1), 0));

    } else if (m_moveTimer <= 0) {
        m_pauseTimer = 2;
    }
}

void Blobby::Render () {
    glPushMatrix();
    Graphics::SetColor(Volt::Color::RGB(50, 0, 200));
    Graphics::TransformMatrix(m_transform);
    glScalef(1, RADIUS, 1);
    Graphics::RenderCircle(0.8, 16);
    glPopMatrix();
}

