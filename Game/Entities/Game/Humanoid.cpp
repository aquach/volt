#include "Game/Entities/Game/Humanoid.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Game/FSM.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/SDLWindow.h"
#include "Volt/Graphics/SpriteAnimation.h"
#include "Volt/Gui/Label.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Entities/Game/Ladder.h"
#include "Game/Entities/Game/Triangle.h"
#include "Game/Entities/Game/Weapon.h"
#include "Game/Graphics/Graphics.h"

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

void Humanoid::IdleState::Update ()  {

    // Left and right movement.
    Vector2 vel = m_h->m_body->GetLinearVelocity();
    if (m_h->m_keyState.IsKeyPressed(SDLK_LEFT) ||
        m_h->m_keyState.IsKeyPressed(SDLK_RIGHT)) {
        float airMult = m_h->IsOnGround() ? 1.0f : AIR_ACCEL;
        int dir = m_h->m_keyState.IsKeyPressed(SDLK_LEFT) ? -1 : 1;
        if (vel.x * dir < MOVE_MAX_VEL) {
            float vx = MOVE_IMPULSE * dir * airMult;
            m_h->m_body->ApplyLinearImpulse(b2Vec2(vx, 0),
                                       m_h->m_body->GetWorldCenter());
        }
    }

    // Jumping.
    m_h->UpdateJump();
}

void Humanoid::IdleState::OnKeyEvent (SDL_KeyboardEvent event) {
    if (event.keysym.sym == SDLK_LEFT || event.keysym.sym == SDLK_RIGHT) {
        if (event.type == SDL_KEYDOWN)
            m_h->m_anim->PlayTrack("run");
        else
            m_h->m_anim->PlayTrack("idle");
    }


    // Climbing ladders.
    if (event.keysym.sym == SDLK_UP || event.keysym.sym == SDLK_DOWN) {
        if (event.type == SDL_KEYDOWN && m_h->m_ladder != NULL) {
            TransitionTo("Ladder");
        }
    }

    // Accessing things.
    if (event.keysym.sym == SDLK_UP) {
        if (event.type == SDL_KEYDOWN) {
            vector<Volt::Entity*> entities;
            m_h->scene()->GetEntitiesAtPoint(m_h->position(), &entities);
            for (uint i = 0; i < entities.size(); i++) {
                if (Entity* e = dynamic_cast<Entity*>(entities[i])) {
                    e->OnAccessed(m_h);
                }
            }
        }
    }

    // Wall jump.
    if (event.keysym.sym == SDLK_z && event.type == SDL_KEYDOWN) {
        if (!m_h->IsOnGround()) {
            SideContact type = m_h->m_body->GetLinearVelocity().x > 0 ?
                               RIGHT : LEFT;
            if (m_h->m_sideContacts[type] != NULL) {
                float dir = SIGNOF(m_h->m_body->GetLinearVelocity().x);
                b2Vec2 impulse(-dir * JUMP_IMPULSE * 5, -JUMP_IMPULSE * 5);
                m_h->m_body->ApplyLinearImpulse(
                    impulse,
                    m_h->m_body->GetWorldCenter());
            }
        }
    }
}


void Humanoid::IdleState::OnEnter () {
}

void Humanoid::IdleState::OnExit () {
}

void Humanoid::LadderState::Update () {
    CHECK_NOTNULL(m_h->m_ladder);

    Vector2 vel = m_h->m_body->GetLinearVelocity();

    // Nullify gravity.
    Vector2 force = Volt::G_PhysicsManager->GetGravity() *
                    m_h->m_body->GetMass();
    m_h->m_body->ApplyForce(-force.ToB2(),
                       m_h->m_body->GetWorldCenter());

    // Movement up and down ladder.
    bool canMove = false;
    if (m_h->m_keyState.IsKeyPressed(SDLK_UP) ||
        m_h->m_keyState.IsKeyPressed(SDLK_DOWN)) {
        int dir = m_h->m_keyState.IsKeyPressed(SDLK_UP) ? -1 : 1;
        if (
            (dir > 0 && m_h->position().y < m_h->m_ladder->bottomY()) ||
            (dir < 0 && m_h->position().y > m_h->m_ladder->topY())
        ) {
            canMove = true;
            if (vel.y * dir < MOVE_MAX_VEL) {
                float vy = MOVE_IMPULSE * dir;
                m_h->m_body->ApplyLinearImpulse(
                    b2Vec2(0, vy),
                    m_h->m_body->GetWorldCenter());
            }
        }
    }

    if (!canMove) {
        m_h->m_body->SetLinearVelocity(b2Vec2(0, 0));
    }
}

void Humanoid::LadderState::OnEnter () {
    // Lock to ladder.
    b2Vec2 v(m_h->m_ladder->position().x, m_h->position().y);
    m_h->m_body->SetTransform(v, 0);
    m_h->m_body->SetLinearVelocity(b2Vec2(0, 0));
}

void Humanoid::LadderState::OnExit () {
}

void Humanoid::LadderState::OnKeyEvent (SDL_KeyboardEvent event) {
    // Getting off ladders.
    if (event.keysym.sym == SDLK_z && event.type == SDL_KEYDOWN) {
        TransitionTo("Normal");
    }
}

Humanoid::Humanoid ()
    : m_jumpTimer(0.0f),
      m_debugDraw(true),
      m_ladder(NULL),
      m_fsm(NULL),
      m_debugLabel(NULL),
      m_anim(NULL),
      m_inputListener(NULL),
      m_superArmor(false),
      m_actionLock(false) {
    AddTag("Humanoid");

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
    m_fsm->AddState(new Humanoid::IdleState(this), "Idle");
    m_fsm->AddState(new Humanoid::LadderState(this), "Ladder");
    m_fsm->TransitionTo("Idle");

    AddContactListener(new ContactListener);

    if (m_debugDraw) {
        m_debugLabel = new Volt::Label(DEBUG_FONT_LARGE,
                                       G_Window->width(), 0);
        m_debugLabel->AddTag("HumanoidStateLabel");
        m_debugLabel->SetAnchor(Volt::Label::ANCHOR_RIGHT,
                                Volt::Label::ANCHOR_TOP);
        m_debugLabel->SetColor(Volt::Color::RGB(255, 255, 0));
    }
}

Humanoid::~Humanoid () {
    if (m_anim != NULL)
        delete m_anim;
    if (m_inputListener != NULL)
        delete m_inputListener;
    delete m_fsm;
}

void Humanoid::OnAdded () {
    if (m_debugDraw) {
        scene()->Add(m_debugLabel, -50);
    }
}

void Humanoid::OnRemoved () {
    Creature::OnRemoved();
    if (m_debugDraw) {
        scene()->Remove(m_debugLabel);
    }
}

bool Humanoid::IsOnGround () const {
    return m_sideContacts[BOTTOM] != NULL;
}

void Humanoid::Update () {
    UpdatePhysics();

    m_fsm->Update();
    m_anim->Update();

    if (m_debugDraw) {
        m_debugLabel->SetText(m_fsm->stateName());
    }
}

void Humanoid::UpdateJump () {
    Vector2 vel = m_body->GetLinearVelocity();
    if (m_keyState.IsKeyPressed(SDLK_z)) {
        if (IsOnGround()) {
            m_jumpTimer = JUMP_TIME;
            m_anim->PlayTrack("jump");
        }

        if (m_jumpTimer > 0) {
            m_jumpTimer -= G_Time->dt();
            if (vel.y > -JUMP_MAX_VEL) {
                m_body->ApplyLinearImpulse(b2Vec2(0, -JUMP_IMPULSE),
                                           m_body->GetWorldCenter());
            }
        }
    }
}

bool Humanoid::OnKeyEvent (SDL_KeyboardEvent event) {
    m_keyState.Update(event);
    if (event.keysym.sym == SDLK_F3)
        m_anim->ReloadSprites();

    // Action triggers.
    if (event.type == SDL_KEYDOWN && !m_actionLock) {
        Action action = ACTION_UNKNOWN;
        // TODO: Use KeyBindings.
        switch (event.keysym.sym) {
            case SDLK_UP: action = ACTION_JUMP; break;
            case SDLK_LEFT: action = ACTION_LEFT; break;
            case SDLK_RIGHT: action = ACTION_RIGHT; break;
            case SDLK_z: action = ACTION_ATTACK; break;
            case SDLK_x: action = ACTION_ATTACK2; break;
            case SDLK_c: action = ACTION_BLOCK; break;
            default: break;
        }
        if (action != ACTION_UNKNOWN) {
            for (uint i = 0; i < m_actionTriggers.size(); i++) {
                Trigger* t = &m_actionTriggers[i];
                if (t->action == action &&
                    t->startStateName == m_fsm->stateName()) {
                    m_fsm->TransitionTo(t->changeToState->name());
                    return true;
                }
            }

            /* If no trigger satisfies, then tell the current state about the
             * action. */
            Humanoid::HumanoidState* state;
            state = dynamic_cast<Humanoid::HumanoidState*>(m_fsm->state());
            CHECK_NOTNULL(state);
            state->OnAction(action);
        }
    }

    // Humanoid consumes all keystrokes since nothing else needs it at a lower
    // priority atm.
    return true;
}

void Humanoid::Render () {
    m_anim->Render();
}

void Humanoid::OnContactBegin (Volt::Entity* other, b2Contact* contact) {
    Ladder* ladder;

    // Hack to limit scope to just triangles. TODO: Should be anything solid?
    if (dynamic_cast<Triangle*>(other) != NULL) {
        b2WorldManifold manifold;
        contact->GetWorldManifold(&manifold);
        Vector2 dir(manifold.normal);
        //LOG(INFO) << dir;
        if (dir.y > 0 && abs(dir.y) > abs(dir.x)) {
            m_sideContacts[BOTTOM] = other->body();
            m_anim->PlayTrack("idle");
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

void Humanoid::OnContactEnd (Volt::Entity* other, b2Contact* contact) {
    if (dynamic_cast<Triangle*>(other) != NULL) {
        for (int i = 0; i < 4; i++)
            if (m_sideContacts[i] == other->body())
                m_sideContacts[i] = NULL;
    } else if (dynamic_cast<Ladder*>(other) != NULL) {
        m_ladder = NULL;
    }
}

void Humanoid::CreateInputListener (GameScene* gameScene) {
    if (m_inputListener != NULL)
        delete m_inputListener;

    m_inputListener = new Humanoid::InputListener(this);
    gameScene->AddInputListener(m_inputListener, 2);
}

void Humanoid::RemoveInputListener () {
    if (m_inputListener == NULL)
        return;
    delete m_inputListener;
}

void Humanoid::HumanoidState::AddActionTrigger (Action action,
                                                const string& stateName) {
    m_h->AddActionTrigger(action, stateName, this);
}

void Humanoid::AddActionTrigger (Action action, const string& stateName,
                                 Humanoid::HumanoidState* state) {
    Trigger t = { action, stateName, state };
    m_actionTriggers.push_back(t);
}
