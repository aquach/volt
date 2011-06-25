#include "Player.h"
#include "Graphics/Graphics.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/OpenGL.h"
#include "Volt/Graphics/Window.h"

const float WIDTH = 1.5f;
const float HEIGHT = 2.0f;
const float AIR_ACCEL = 0.15f;
const float MOVE_IMPULSE = 3.0f;
const float MOVE_MAX_VEL = 8.0f;
const float JUMP_TIME = 0.1f;
const float JUMP_IMPULSE = 10.0f;
const float JUMP_MAX_VEL = 10.0f;

enum SideContact {
    TOP,
    LEFT,
    RIGHT,
    BOTTOM
};

Player::Player ()
    : m_jumpTimer(0.0f),
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

    memset(m_sideContacts, 0, sizeof(m_sideContacts));
}

Player::~Player () {
}

bool Player::IsOnGround () const {
    return m_sideContacts[BOTTOM] != NULL;
}

void Player::Update () {
    UpdatePhysics();

    Vector2 vel = m_body->GetLinearVelocity();
    if (Volt::G_Window->IsKeyPressed(SDLK_LEFT) ||
        Volt::G_Window->IsKeyPressed(SDLK_RIGHT)) {
        float airMult = IsOnGround() ? 1.0f : AIR_ACCEL;
        int dir = Volt::G_Window->IsKeyPressed(SDLK_LEFT) ? -1 : 1;
        if (vel.x * dir < MOVE_MAX_VEL) {
            float vx = MOVE_IMPULSE * dir * airMult;
            m_body->ApplyLinearImpulse(b2Vec2(vx, 0), m_body->GetWorldCenter());
        }
    }
    if (Volt::G_Window->IsKeyPressed(SDLK_z)) {
        if (IsOnGround())
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
        if (!IsOnGround()) {
            SideContact type = m_body->GetLinearVelocity().x > 0 ? RIGHT : LEFT;
            if (m_sideContacts[type] != NULL) {
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
    }
}

void Player::BeginContact (Entity* other, b2Contact* contact) {
    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);
    Vector2 dir(manifold.normal);
    if (dir.x > 0 && abs(dir.x) > abs(dir.y)) {
        m_sideContacts[RIGHT] = other->body();
    } else if (dir.x < 0 && abs(dir.x) > abs(dir.y)) {
        m_sideContacts[LEFT] = other->body();
    } else if (dir.y < 0 && abs(dir.y) > abs(dir.x)) {
        m_sideContacts[TOP] = other->body();
    } else if (dir.y > 0 && abs(dir.y) > abs(dir.x)) {
        m_sideContacts[BOTTOM] = other->body();
    }
}

void Player::EndContact (Entity* other, b2Contact* contact) {
    for (int i = 0; i < 4; i++)
        if (m_sideContacts[i] == other->body())
            m_sideContacts[i] = NULL;

}
