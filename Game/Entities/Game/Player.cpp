#include "Player.h"
#include "Graphics/Graphics.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/OpenGL.h"
#include "Volt/Graphics/Window.h"

class SurfaceRayCastCallback : public b2RayCastCallback {
public:
    SurfaceRayCastCallback (Player* player)
        : m_player(player),
          m_body(NULL) { }

    float32 ReportFixture (b2Fixture* fixture, const b2Vec2& point,
                           const b2Vec2& normal, float32 fraction) {
        b2Body* body = fixture->GetBody();
        if (m_player->body() != body && body->GetType() == b2_staticBody) {
            m_body = body;
            m_point = point;
            m_normal = normal;
            return fraction;
        }
        return -1;
    }

    b2Body* m_body;
    Vector m_point;
    Vector m_normal;
private:
    Player* m_player;
};

const float WIDTH = 1.5f;
const float HEIGHT = 2.0f;
const float AIR_ACCEL = 0.2f;
const float MOVE_IMPULSE = 3.0f;
const float MOVE_MAX_VEL = 8.0f;
const float JUMP_TIME = 0.1f;
const float JUMP_IMPULSE = 10.0f;
const float JUMP_MAX_VEL = 10.0f;

Player::Player ()
    : m_onGround(false),
      m_jumpTimer(0.0f),
      m_debugDraw(true) {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.fixedRotation = true;
    m_body = CreateBody(def);

    b2PolygonShape shape;
    shape.SetAsBox(WIDTH * 0.5f, HEIGHT * 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.friction = 1.0f;
    m_body->CreateFixture(&fixtureDef);
}

Player::~Player () {
}

b2AABB Player::GetGroundTestAABB () {
    b2AABB aabb;
    aabb.lowerBound = (m_transform.position - Vector2(0.1, 0)).ToB2();
    aabb.upperBound = (m_transform.position +
                       Vector2(0.1, HEIGHT * 0.5f - 0.01)).ToB2();
    return aabb;
}

void Player::OnGroundCheck () {
    m_onGround = false;
    SurfaceRayCastCallback callback(this);
    b2AABB aabb = GetGroundTestAABB();
    Volt::G_PhysicsManager->world()->QueryAABB(&callback, aabb);
    LOG(INFO) << callback.m_body;
    if (callback.m_body != NULL)
        m_onGround = true;
}

void Player::Update () {
    UpdatePhysics();
    OnGroundCheck();

    Vector2 vel = m_body->GetLinearVelocity();
    if (Volt::G_Window->IsKeyPressed(SDLK_LEFT) ||
        Volt::G_Window->IsKeyPressed(SDLK_RIGHT)) {
        float airMult = m_onGround ? 1.0f : AIR_ACCEL;
        int dir = Volt::G_Window->IsKeyPressed(SDLK_LEFT) ? -1 : 1;
        if (vel.x * dir < MOVE_MAX_VEL) {
            float vx = MOVE_IMPULSE * dir * airMult;
            m_body->ApplyLinearImpulse(b2Vec2(vx, 0), m_body->GetWorldCenter());
        }
    }
    if (Volt::G_Window->IsKeyPressed(SDLK_z)) {
        if (m_onGround)
            m_jumpTimer = JUMP_TIME;
        if (m_jumpTimer > 0) {
            m_jumpTimer -= Volt::G_Game->dt();
            if (vel.y > -JUMP_MAX_VEL) {
                m_body->ApplyLinearImpulse(b2Vec2(0, -JUMP_IMPULSE),
                                           m_body->GetWorldCenter());
            }
        }
    }
}

void Player::OnKeyEvent (SDL_KeyboardEvent event) {
    if (event.keysym.sym == SDLK_z && event.type == SDL_KEYDOWN) {
        if (!m_onGround) {
            LOG(INFO) << "POW";
            SurfaceQueryCallback callback(this);
            b2AABB aabb;
            if (m_body->GetLinearVelocity().x > 0) {
                aabb.lowerBound = (m_transform.position - Vector2(0, 0.01)).ToB2();
                aabb.upperBound = (m_transform.position +
                                   Vector2(WIDTH * 0.5f, 0.1)).ToB2();
            } else {
                aabb.lowerBound = (m_transform.position -
                                   Vector2(WIDTH * 0.5f - 0.01, 0.1)).ToB2();
                aabb.upperBound = (m_transform.position + Vector2(0, 0.01)).ToB2();
            }
            Volt::G_PhysicsManager->world()->QueryAABB(&callback, aabb);
            if (callback.m_body != NULL) {
                LOG(INFO) << "THING";
                float dir = SIGN(m_body->GetLinearVelocity().x, 1);
                m_body->ApplyLinearImpulse(b2Vec2(-dir * JUMP_IMPULSE * 5,
                                                  -JUMP_IMPULSE * 5),
                                           m_body->GetWorldCenter());
            }
        }
    }
}

void Player::Render () {
    Graphics::SetBlend(Graphics::BLEND_NONE);
    glLineWidth(2.0f);
    glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderQuad(WIDTH, HEIGHT);
    glPopMatrix();

    if (m_debugDraw) {
        Graphics::SetColor(Volt::Color::white);
        b2AABB aabb = GetGroundTestAABB();
        Volt::BBox box(aabb.lowerBound, aabb.upperBound);
        Graphics::RenderLineRect(box.center().x, box.center().y,
                                 box.extents().x, box.extents().y);
    }
}
