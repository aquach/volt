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

// Constants - TODO: maybe change to defaults + changeable.
const float AIR_ACCEL = 0.15f;
const float MOVE_IMPULSE = 3.0f;
const float JUMP_IMPULSE = 10.0f;

// Defaults.
const float RUN_SPEED = 8.0f;
const float JUMP_TIME = 0.1f;
const float JUMP_SPEED = 10.0f;

enum SideContact {
    TOP,
    LEFT,
    RIGHT,
    BOTTOM
};

Humanoid::HumanoidState::~HumanoidState () {
    m_h->RemoveActionTriggers(this);
}

void Humanoid::HumanoidState::Update () {
    m_stateTime += G_Time->dt();
}

void Humanoid::IdleState::OnEnter () {
    m_h->PlayAnimTrack("idle");
}

Humanoid::RunningState::RunningState (Humanoid* humanoid)
    : HumanoidState(humanoid) {
    AddActionTrigger(ACTION_LEFT, NULL);
    AddActionTrigger(ACTION_RIGHT, NULL);
}

void Humanoid::RunningState::Update () {
    Humanoid::HumanoidState::Update();

    m_h->PlayAnimTrack("run");

    // Left and right movement.
    int dir = 0;
    if (m_h->m_keyState.IsKeyPressed(SDLK_LEFT))
        dir = -1;
    else if (m_h->m_keyState.IsKeyPressed(SDLK_RIGHT))
        dir = 1;

    if (dir != 0) {
        m_h->Run(dir);
    } else {
        TransitionTo("Idle");
    }
}


bool Humanoid::LadderState::LadderTriggerTest::CanTransition () {
    Ladder* l = m_state->FindLadder();
    return l != NULL;
}

Ladder* Humanoid::LadderState::FindLadder () {
    vector<Volt::Entity*> entities;
    Vector2 halfExtents(WIDTH / 2, HEIGHT / 2);
    m_h->scene()->GetEntitiesInArea(m_h->position() - halfExtents,
                                    m_h->position() + halfExtents,
                                    &entities);
    for (uint i = 0; i < entities.size(); i++) {
        if (Ladder* l = dynamic_cast<Ladder*>(entities[i])) {
            return l;
        }
    }
    return NULL;
}

Humanoid::LadderState::LadderState (Humanoid* humanoid)
    : HumanoidState(humanoid) {
    AddActionTrigger(ACTION_UP, new LadderTriggerTest(this));
}

void Humanoid::LadderState::Update () {
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
            (dir > 0 && m_h->position().y < m_ladder->bottomY()) ||
            (dir < 0 && m_h->position().y > m_ladder->topY())
        ) {
            canMove = true;
            if (vel.y * dir < RUN_SPEED) {
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
    m_ladder = FindLadder();
    CHECK_NOTNULL(m_ladder);
    // Lock to ladder.
    b2Vec2 v(m_ladder->position().x, m_h->position().y);
    m_h->m_body->SetTransform(v, 0);
    m_h->m_body->SetLinearVelocity(b2Vec2(0, 0));
    m_h->PlayAnimTrack("idle"); // TODO: Ladder anim.
}

void Humanoid::LadderState::OnExit () {
    m_ladder = NULL;
}

void Humanoid::LadderState::OnAction (Action action) {
    if (action == ACTION_JUMP)
        TransitionTo("Idle");
}

Humanoid::Strike1State::Strike1State (Humanoid* humanoid)
    : HumanoidState(humanoid) {
    AddActionTrigger(ACTION_ATTACK, NULL);
}

void Humanoid::Strike1State::Update () {
    if (m_h->m_anim->completed())
        TransitionTo("Idle");
}

void Humanoid::Strike1State::OnEnter () {
    m_h->m_anim->PlayTrack("strike1");
}

void Humanoid::Strike1State::OnStruckEnemy (Creature* enemy) {
    LOG(INFO) << "HURT A " << *enemy;
    enemy->TakeDamage(m_h, 1);
}

Humanoid::Humanoid ()
    : m_jumpTimer(0.0f),
      m_debugDraw(true),
      m_fsm(NULL),
      m_debugLabel(NULL),
      m_anim(NULL),
      m_inputListener(NULL),
      m_superArmor(false),
      m_actionLock(false),
      m_runSpeed(RUN_SPEED),
      m_jumpSpeed(JUMP_SPEED),
      m_jumpTime(JUMP_TIME) {
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
    m_fsm->AddState(new Humanoid::RunningState(this), "Running");
    m_fsm->AddState(new Humanoid::Strike1State(this), "Strike1");

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
    m_fsm->TransitionTo("Idle");
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

    if (m_keyState.IsKeyPressed(SDLK_z) && !m_actionLock) {
        Jump();
    }

    // Check hitboxes.
    HumanoidState* state = dynamic_cast<HumanoidState*>(m_fsm->state());
    const Json::Value& data = m_anim->frameUserData();
    if (!data.empty()) {
        const Json::Value& hitboxes = data.get("hitboxes", "");
        for (uint i = 0; i < hitboxes.size(); i++) {
            Volt::BBox b;
            b.Load(hitboxes[i]);

            b.min += position();
            b.max += position();

            vector<Volt::Entity*> entities;
            scene()->GetEntitiesInArea(b.min, b.max, &entities);
            for (uint i = 0; i < entities.size(); i++) {
                Creature* c = dynamic_cast<Creature*>(entities[i]);
                if (entities[i] != this && c != NULL) {
                    state->OnStruckEnemy(c);
                }
            }
        }
    }

    if (m_debugDraw) {
        m_debugLabel->SetText(m_fsm->stateName());
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
            case SDLK_z: action = ACTION_JUMP; break;
            case SDLK_UP: action = ACTION_UP; break;
            case SDLK_LEFT: action = ACTION_LEFT; break;
            case SDLK_RIGHT: action = ACTION_RIGHT; break;
            case SDLK_DOWN: action = ACTION_DOWN; break;
            case SDLK_x: action = ACTION_ATTACK; break;
            case SDLK_c: action = ACTION_ATTACK2; break;
            case SDLK_v: action = ACTION_BLOCK; break;
            default: break;
        }
        LOG(INFO) << "ACTION: " << action;
        if (action != ACTION_UNKNOWN) {
            for (uint i = 0; i < m_actionTriggers.size(); i++) {
                Trigger* t = &m_actionTriggers[i];
                if (t->action == action && t->test->CanTransition()) {
                        LOG(INFO) << "Trigger state " << t->changeToState->name();
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

            // Wall jumping.
            if (action == ACTION_JUMP) {
                WallJump();
            } else if (action == ACTION_DOWN) {
                vector<Volt::Entity*> entities;
                scene()->GetEntitiesAtPoint(position(), &entities);
                for (uint i = 0; i < entities.size(); i++) {
                    if (Entity* e = dynamic_cast<Entity*>(entities[i])) {
                        e->OnAccessed(this);
                    }
                }
            }
        }
    }

    // Humanoid consumes all keystrokes since nothing else needs it at a lower
    // priority atm.
    return true;
}

void Humanoid::Render () {
    m_anim->Render();
    if (m_debugDraw) {
        // Draw hitboxes.
        Graphics::SetColor(Volt::Color::RGBA(255, 0, 0, 128));

        const Json::Value& data = m_anim->frameUserData();
        if (!data.empty()) {
            const Json::Value& hitboxes = data.get("hitboxes", "");
            for (uint i = 0; i < hitboxes.size(); i++) {
                Volt::BBox b;
                b.Load(hitboxes[i]);

                glPushMatrix();
                Graphics::Translate(b.center() + position());
                Vector2 extents = b.extents();
                Graphics::RenderQuad(extents.x, extents.y);
                glPopMatrix();
            }
        }
    }
}

void Humanoid::OnContactBegin (Volt::Entity* other, b2Contact* contact) {
    // Hack to limit scope to just triangles. TODO: Should be anything solid?
    if (dynamic_cast<Triangle*>(other) != NULL) {
        b2WorldManifold manifold;
        contact->GetWorldManifold(&manifold);
        Vector2 dir(manifold.normal);
        //LOG(INFO) << dir;
        if (dir.y > 0 && abs(dir.y) > abs(dir.x)) {
            m_sideContacts[BOTTOM] = other->body();
        } else if (dir.x > 0 && abs(dir.x) > abs(dir.y)) {
            m_sideContacts[RIGHT] = other->body();
        } else if (dir.x < 0 && abs(dir.x) > abs(dir.y)) {
            m_sideContacts[LEFT] = other->body();
        } else if (dir.y < 0 && abs(dir.y) > abs(dir.x)) {
            m_sideContacts[TOP] = other->body();
        }
    }
}

void Humanoid::OnContactEnd (Volt::Entity* other, b2Contact* contact) {
    if (dynamic_cast<Triangle*>(other) != NULL) {
        for (int i = 0; i < 4; i++)
            if (m_sideContacts[i] == other->body())
                m_sideContacts[i] = NULL;
    }
}

void Humanoid::CreateInputListener (GameScene* gameScene) {
    if (m_inputListener != NULL)
        delete m_inputListener;

    m_inputListener = new Humanoid::InputListener(this);
    gameScene->AddInputListener(m_inputListener, 3);
}

void Humanoid::RemoveInputListener () {
    if (m_inputListener != NULL)
        delete m_inputListener;
}

void Humanoid::HumanoidState::AddActionTrigger (Action action,
                                                ActionTriggerTest* test) {
    // Default test passes always.
    if (test == NULL)
        test = new ActionTriggerTest;
    m_h->AddActionTrigger(action, test, this);
}

void Humanoid::AddActionTrigger (Action action, ActionTriggerTest* test,
                                 Humanoid::HumanoidState* state) {
    Trigger t = { action, test, state };
    m_actionTriggers.push_back(t);
}

void Humanoid::RemoveActionTriggers (Humanoid::HumanoidState* state) {
    for (vector<Trigger>::iterator i = m_actionTriggers.begin();
        i != m_actionTriggers.end();) {
        if (i->changeToState == state) {
            delete i->test;
            i = m_actionTriggers.erase(i);
        } else {
            i++;
        }
    }
}

void Humanoid::AddState (Humanoid::HumanoidState* state,
                         const string& stateName) {
    m_fsm->AddState(state, stateName);
}

void Humanoid::PlayAnimTrack (const string& name) {
    if (m_anim != NULL)
        m_anim->PlayTrack(name);
}

void Humanoid::Run (int dir) {
    float airMult = IsOnGround() ? 1.0f : AIR_ACCEL;
    b2Vec2 vel = m_body->GetLinearVelocity();
    if (vel.x * dir < abs(m_runSpeed)) {
        float vx = MOVE_IMPULSE * dir * airMult;
        m_body->ApplyLinearImpulse(b2Vec2(vx, 0),
                                   m_body->GetWorldCenter());
    }
}

void Humanoid::Jump () {
    Vector2 vel = m_body->GetLinearVelocity();

    // TODO: Can't jump off ladders because of this check.
    if (IsOnGround()) {
        m_jumpTimer = m_jumpTime;
        m_anim->PlayTrack("jump");
    }

    if (m_jumpTimer > 0) {
        m_jumpTimer -= G_Time->dt();
        if (vel.y > -m_jumpSpeed) {
            m_body->ApplyLinearImpulse(b2Vec2(0, -JUMP_IMPULSE),
                                       m_body->GetWorldCenter());
        }
    }
}

void Humanoid::ApplyVelocity (Vector2 vel) {
    m_body->ApplyLinearImpulse((vel * m_body->GetMass()).ToB2(),
                               m_body->GetWorldCenter());
}

void Humanoid::WallJump () {
    if (!IsOnGround()) {
        SideContact type = m_body->GetLinearVelocity().x > 0 ? RIGHT : LEFT;
        if (m_sideContacts[type] != NULL) {
            float dir = SIGNOF(m_body->GetLinearVelocity().x);
            b2Vec2 impulse(-dir * JUMP_IMPULSE * 5, -JUMP_IMPULSE * 5);
            m_body->ApplyLinearImpulse(impulse, m_body->GetWorldCenter());
        }
    }
}
