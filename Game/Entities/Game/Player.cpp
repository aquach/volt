#include "Game/Entities/Game/Player.h"
#include "Game/Entities/Game/Ladder.h"
#include "Game/Entities/Game/Triangle.h"
#include "Game/Entities/Game/Weapon.h"
#include "Game/Entities/GUI/HealthBar.h"
#include "Game/Entities/GUI/PowerBar.h"
#include "Game/Graphics/Graphics.h"
#include "Game/Scenes/GameScene.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Game/FSM.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/SDLWindow.h"
#include "Volt/GUI/Label.h"
#include "Game/Editor/EntityFactory.h"

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

void Player::NormalState::Update ()  {

    // Left and right movement.
    Vector2 vel = m_p->m_body->GetLinearVelocity();
    if (!m_p->m_inputLock) {
        if (G_Window->IsKeyPressed(SDLK_LEFT) ||
            G_Window->IsKeyPressed(SDLK_RIGHT)) {
            float airMult = m_p->IsOnGround() ? 1.0f : AIR_ACCEL;
            int dir = G_Window->IsKeyPressed(SDLK_LEFT) ? -1 : 1;
            if (vel.x * dir < MOVE_MAX_VEL) {
                float vx = MOVE_IMPULSE * dir * airMult;
                m_p->m_body->ApplyLinearImpulse(b2Vec2(vx, 0),
                                           m_p->m_body->GetWorldCenter());
            }
        }
    }

    // Jumping.
    m_p->UpdateJump();
}

void Player::NormalState::OnKeyEvent (SDL_KeyboardEvent event) {
    // Climbing ladders.
    if (event.keysym.sym == SDLK_UP || event.keysym.sym == SDLK_DOWN) {
        if (event.type == SDL_KEYDOWN && m_p->m_ladder != NULL) {
            TransitionTo("Ladder");
        }
    }

    // Accessing things.
    if (event.keysym.sym == SDLK_UP) {
        if (event.type == SDL_KEYDOWN) {
            vector<Volt::Entity*> entities;
            m_p->scene()->GetEntitiesAtPoint(m_p->position(), &entities);
            for (uint i = 0; i < entities.size(); i++) {
                if (Entity* e = dynamic_cast<Entity*>(entities[i])) {
                    e->OnAccessed(m_p);
                }
            }
        }
    }

    // Wall jump.
    if (event.keysym.sym == SDLK_z && event.type == SDL_KEYDOWN) {
        if (!m_p->IsOnGround()) {
            SideContact type = m_p->m_body->GetLinearVelocity().x > 0 ?
                               RIGHT : LEFT;
            if (m_p->m_sideContacts[type] != NULL) {
                float dir = SIGNOF(m_p->m_body->GetLinearVelocity().x);
                b2Vec2 impulse(-dir * JUMP_IMPULSE * 5, -JUMP_IMPULSE * 5);
                m_p->m_body->ApplyLinearImpulse(
                    impulse,
                    m_p->m_body->GetWorldCenter());
            }
        }
    }
}


void Player::NormalState::OnEnter () {
}

void Player::NormalState::OnExit () {
}

void Player::LadderState::Update () {
    CHECK_NOTNULL(m_p->m_ladder);

    Vector2 vel = m_p->m_body->GetLinearVelocity();

    // Nullify gravity.
    Vector2 force = Volt::G_PhysicsManager->GetGravity() *
                    m_p->m_body->GetMass();
    m_p->m_body->ApplyForce(-force.ToB2(),
                       m_p->m_body->GetWorldCenter());

    // Movement up and down ladder.
    bool canMove = false;
    if (!m_p->m_inputLock) {
        if (G_Window->IsKeyPressed(SDLK_UP) ||
            G_Window->IsKeyPressed(SDLK_DOWN)) {
            int dir = G_Window->IsKeyPressed(SDLK_UP) ? -1 : 1;
            if (
                (dir > 0 && m_p->position().y < m_p->m_ladder->bottomY()) ||
                (dir < 0 && m_p->position().y > m_p->m_ladder->topY())
            ) {
                canMove = true;
                if (vel.y * dir < MOVE_MAX_VEL) {
                    float vy = MOVE_IMPULSE * dir;
                    m_p->m_body->ApplyLinearImpulse(
                        b2Vec2(0, vy),
                        m_p->m_body->GetWorldCenter());
                }
            }
        }
    }

    if (!canMove) {
        m_p->m_body->SetLinearVelocity(b2Vec2(0, 0));
    }
}

void Player::LadderState::OnEnter () {
    // Lock to ladder.
    b2Vec2 v(m_p->m_ladder->position().x, m_p->position().y);
    m_p->m_body->SetTransform(v, 0);
    m_p->m_body->SetLinearVelocity(b2Vec2(0, 0));
}

void Player::LadderState::OnExit () {
}

void Player::LadderState::OnKeyEvent (SDL_KeyboardEvent event) {
    // Getting off ladders.
    if (event.keysym.sym == SDLK_z && event.type == SDL_KEYDOWN) {
        TransitionTo("Normal");
    }
}

Player::Player ()
    : m_jumpTimer(0.0f),
      m_debugDraw(true),
      m_ladder(NULL),
      m_fsm(NULL),
      m_debugLabel(NULL),
      m_healthBar(NULL),
      m_powerBar(NULL),
      m_inputLock(false) {
    AddTag("Player");

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
    fixtureDef.friction = 0.5f;
    m_body->CreateFixture(&fixtureDef);

    memset(m_sideContacts, 0, sizeof(m_sideContacts));

    m_fsm = new Volt::FSM;
    m_fsm->AddState(new Player::NormalState(this), "Normal");
    m_fsm->AddState(new Player::LadderState(this), "Ladder");
    m_fsm->TransitionTo("Normal");

    ContactListener* listener = new ContactListener(this);
    AddContactListener(listener);

    if (m_debugDraw) {
        m_debugLabel = new Volt::Label(DEBUG_FONT_LARGE,
                                       G_Window->width(), 0);
        m_debugLabel->AddTag("PlayerStateLabel");
        m_debugLabel->SetAnchor(Volt::Label::ANCHOR_RIGHT,
                                Volt::Label::ANCHOR_TOP);
        m_debugLabel->SetColor(Volt::Color::RGB(255, 255, 0));
    }

    m_healthBar = new HealthBar(m_health / m_maxHealth, 50 /* Speed */);
    m_powerBar = new PowerBar(m_power / m_maxPower, 50 /* Speed */);

    m_weaponTransform.position.Set(0.4, 0);
}

void Player::OnAdded () {
    if (m_debugDraw) {
        scene()->Add(m_debugLabel, -50);
    }
    scene()->Add(m_healthBar, -40);
    scene()->Add(m_powerBar, -40);
}

void Player::OnRemoved () {
    Creature::OnRemoved();
    if (m_debugDraw) {
        scene()->Remove(m_debugLabel);
    }
    scene()->Remove(m_healthBar);
    scene()->Remove(m_powerBar);
}

bool Player::IsOnGround () const {
    return m_sideContacts[BOTTOM] != NULL;
}

void Player::Update () {
    UpdatePhysics();

    m_fsm->Update();

    if (m_debugDraw) {
        m_debugLabel->SetText(m_fsm->stateName());
    }
}

void Player::UpdateJump () {
    Vector2 vel = m_body->GetLinearVelocity();
    if (!m_inputLock && G_Window->IsKeyPressed(SDLK_z)) {
        if (IsOnGround())
            m_jumpTimer = JUMP_TIME;

        if (m_jumpTimer > 0) {
            m_jumpTimer -= G_Time->dt();
            if (vel.y > -JUMP_MAX_VEL) {
                m_body->ApplyLinearImpulse(b2Vec2(0, -JUMP_IMPULSE),
                                           m_body->GetWorldCenter());
            }
        }
    }
}

void Player::OnKeyEvent (SDL_KeyboardEvent event) {
    if (m_inputLock)
        return;
    Player::PlayerState* state;
    state = dynamic_cast<Player::PlayerState*>(m_fsm->state());
    CHECK_NOTNULL(state);
    state->OnKeyEvent(event);
}

void Player::Render () {
    glLineWidth(2.0f);
    glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderQuad(WIDTH, HEIGHT);
    glPopMatrix();
}

void Player::OnContactBegin (Volt::Entity* other, b2Contact* contact) {
    Ladder* ladder;

    // Hack to limit scope to just triangles.
    if (dynamic_cast<Triangle*>(other) != NULL) {
        b2WorldManifold manifold;
        contact->GetWorldManifold(&manifold);
        Vector2 dir(manifold.normal);
        LOG(INFO) << dir;
        if (dir.y > 0 && abs(dir.y) > abs(dir.x)) {
            m_sideContacts[BOTTOM] = other->body();
        } else if (dir.x > 0 && abs(dir.x) > abs(dir.y)) {
            m_sideContacts[RIGHT] = other->body();
        } else if (dir.x < 0 && abs(dir.x) > abs(dir.y)) {
            m_sideContacts[LEFT] = other->body();
        } else if (dir.y < 0 && abs(dir.y) > abs(dir.x)) {
            m_sideContacts[TOP] = other->body();
        }
    } else if ((ladder = dynamic_cast<Ladder*>(other)) != NULL) {
        m_ladder = ladder;
    }
}

void Player::OnContactEnd (Volt::Entity* other, b2Contact* contact) {
    if (dynamic_cast<Triangle*>(other) != NULL) {
        for (int i = 0; i < 4; i++)
            if (m_sideContacts[i] == other->body())
                m_sideContacts[i] = NULL;
    } else if (dynamic_cast<Ladder*>(other) != NULL) {
        m_ladder = NULL;
    }
}
