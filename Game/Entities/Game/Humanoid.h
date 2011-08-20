#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/FSM.h"
#include "Volt/Game/KeyState.h"
#include "Volt/Graphics/Input.h"
#include "Game/Entities/Game/Creature.h"
#include "Game/Scenes/GameScene.h"

namespace Volt {
    class Label;
    class SpriteAnimation;
};

class Ladder;

class Humanoid : public Creature {
public:
    enum Action {
        ACTION_UNKNOWN = -1,
        ACTION_JUMP,
        ACTION_LEFT,
        ACTION_RIGHT,
        ACTION_ATTACK,
        ACTION_ATTACK2,
        ACTION_BLOCK
    };

    class HumanoidState : public Volt::FSMState {
    public:
        explicit HumanoidState (Humanoid* Humanoid)
            : m_h(Humanoid) { }

        virtual void Update () = 0;
        virtual void OnEnter () = 0;
        virtual void OnExit () = 0;
        virtual void OnStruckEnemy () = 0;
        //virtual void OnStruck () = 0;
        virtual void OnAction (Action action) = 0;

        /* Possibly add multiple actions and state names in AND/OR combo? */
        void AddActionTrigger (Action action, const string& stateName);

    protected:
        Humanoid* m_h;

    private:
        DISALLOW_COPY_AND_ASSIGN(HumanoidState);
    };

    class IdleState : public HumanoidState {
    public:
        explicit IdleState (Humanoid* Humanoid)
            : HumanoidState(Humanoid) { }
        virtual void Update ();
        virtual void OnEnter ();
        virtual void OnExit ();
        virtual void OnStruckEnemy () { }
        //virtual void OnStruck () { }
        virtual void OnAction (Action action) { }

        virtual void OnKeyEvent (SDL_KeyboardEvent event);
    };

    class LadderState : public HumanoidState {
    public:
        explicit LadderState (Humanoid* Humanoid)
            : HumanoidState(Humanoid) { }
        virtual void Update ();
        virtual void OnEnter ();
        virtual void OnExit ();
        virtual void OnStruckEnemy () { }
        //virtual void OnStruck () { }
        virtual void OnAction (Action action) { }

        virtual void OnKeyEvent (SDL_KeyboardEvent event);
    };

    Humanoid ();
    virtual ~Humanoid ();

    virtual void Update ();
    virtual void Render ();

    virtual void OnAdded ();
    virtual void OnRemoved ();

    bool IsOnGround () const;

    /* External physics controls on the humanoid that HumanoidStates can use. */
    void Run (float maxSpeed);
    void Slide (float amount, float time);
    void ApplyVelocity (Vector2 vel);
    void Jump (float speed);
    void Fall ();
    void HitPause (float time); /* Pause when hit or being hit. */
    void Recover (float time); /* Recovery phase of being hit */
    /* MORE */

    /* Super armor means that you do not need to recover and cannot fall. */
    void SetSuperArmor (bool enabled) { m_superArmor = enabled; }
    bool superArmor () const { return m_superArmor; }

    /* Action lock means that action triggers do not fire, so you cannot
     * interrupt a state instantly.
     */
    void SetActionLock (bool enabled) { m_actionLock = enabled; }
    bool actionLocked () const { return m_actionLock; }

    void SetAnimation (Volt::SpriteAnimation* animation) { m_anim = animation; }
    void AddState (Humanoid::HumanoidState* state);

    void CreateInputListener (GameScene* gameScene);
    void RemoveInputListener ();

private:
    class ContactListener : public Volt::EntityContactListener {
    public:
        explicit ContactListener () { }
        virtual void OnContactBegin (Volt::Entity* other, b2Contact* contact) {
            Humanoid* h = dynamic_cast<Humanoid*>(entity());
            if (h != NULL)
                h->OnContactBegin(other, contact);
        }
        virtual void OnContactEnd (Volt::Entity* other, b2Contact* contact) {
            Humanoid* h = dynamic_cast<Humanoid*>(entity());
            if (h != NULL)
                h->OnContactEnd(other, contact);
        }
    };

    class InputListener : public GameScene::GameInputListener {
    public:
        InputListener (Humanoid* h) : m_h(h) { }
        virtual ~InputListener () { }
        virtual bool OnKeyEvent (SDL_KeyboardEvent event) {
            return m_h->OnKeyEvent(event);
        }
    private:
        Humanoid* m_h;
    };

    /* Physics */
    void OnContactBegin (Volt::Entity* other, b2Contact* contact);
    void OnContactEnd (Volt::Entity* other, b2Contact* contact);
    void UpdateJump ();

    b2Body* m_sideContacts[4];
    float m_jumpTimer;
    Ladder* m_ladder;
    bool m_superArmor;

    /* States */
    void AddActionTrigger (Action action, const string& stateName,
                           Humanoid::HumanoidState* state);
    Volt::FSM* m_fsm;
    bool m_actionLock;
    struct Trigger {
        Action action;
        string startStateName;
        Humanoid::HumanoidState* changeToState;
    };
    vector<Trigger> m_actionTriggers;

    /* Animation */
    Volt::SpriteAnimation* m_anim;

    /* GUI */
    bool m_debugDraw;
    Volt::Label* m_debugLabel;

    /* Input */
    bool OnKeyEvent (SDL_KeyboardEvent event);
    InputListener* m_inputListener;
    Volt::KeyState m_keyState;

    DISALLOW_COPY_AND_ASSIGN(Humanoid);
};
