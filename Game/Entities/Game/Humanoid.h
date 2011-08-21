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
        ACTION_UP,
        ACTION_LEFT,
        ACTION_RIGHT,
        ACTION_DOWN,
        ACTION_ATTACK,
        ACTION_ATTACK2,
        ACTION_BLOCK
    };

    /* A test that is invoked when checking whether or not it's possible to
     * an action trigger to fire. Allows customization for various states
     * that require certain things to be true before allowing state enter.
     */
    class ActionTriggerTest {
    public:
        virtual bool CanTransition () { return true; }
    };

    class HumanoidState : public Volt::FSMState {
    public:
        explicit HumanoidState (Humanoid* Humanoid)
            : m_h(Humanoid) { }
        virtual ~HumanoidState ();

        virtual void Update ();
        virtual void OnEnter () { m_stateTime = 0; }
        virtual void OnExit () = 0;
        virtual void OnStruckEnemy () = 0;
        //virtual void OnStruck () = 0;
        virtual void OnAction (Action action) = 0;

        /* Adds an action trigger to the host Humanoid, so that when the
         * specified action is executed and the ActionTriggerTest returns
         * true, the Humanoid will immediately switch to this state (provided
         * action lock is off). This provides a mechanism for entering various
         * states based on actions, like climbing a ladder, attacking, running,
         * etc regardless of what state the Humanoid is currently in. For
         * combo-based states where the original state matters, see OnAction.
         */
        void AddActionTrigger (Action action, ActionTriggerTest* test);

        float stateTime () const { return m_stateTime; }

    protected:
        Humanoid* m_h;

    private:
        float m_stateTime;
        DISALLOW_COPY_AND_ASSIGN(HumanoidState);
    };

    Humanoid ();
    virtual ~Humanoid ();

    virtual void Update ();
    virtual void Render ();

    virtual void OnAdded ();
    virtual void OnRemoved ();

    bool IsOnGround () const;

    /* External physics controls on the humanoid that HumanoidStates can use. */
    void Run (int dir);
    void Slide (float amount, float time);
    void ApplyVelocity (Vector2 vel);
    void Jump ();
    void WallJump ();
    void Fall ();
    void HitPause (float time); /* Pause when hit or being hit. */
    void Recover (float time); /* Recovery phase of being hit */
    /* MORE */

    void SetRunSpeed (float speed) { m_runSpeed = speed; }
    bool runSpeed () const { return m_runSpeed; }

    void SetJumpSpeed (float speed) { m_jumpSpeed = speed; }
    bool jumpSpeed () const { return m_jumpSpeed; }

    void SetJumpTime (float speed) { m_jumpTime = speed; }
    bool jumpTime () const { return m_jumpTime; }

    /* Super armor means that you do not need to recover and cannot fall. */
    void SetSuperArmor (bool enabled) { m_superArmor = enabled; }
    bool superArmor () const { return m_superArmor; }

    /* Action lock means that action triggers do not fire, so you cannot
     * interrupt a state instantly.
     */
    void SetActionLock (bool enabled) { m_actionLock = enabled; }
    bool actionLocked () const { return m_actionLock; }

    void SetAnimation (Volt::SpriteAnimation* animation) { m_anim = animation; }
    void AddState (Humanoid::HumanoidState* state, const string& stateName);

    /* Creates an input listener to the scene so that the user's input will
     * affect this Humanoid.
     */
    void CreateInputListener (GameScene* gameScene);
    void RemoveInputListener ();

private:
    // Starting states for Humanoid.
    class IdleState : public HumanoidState {
    public:
        explicit IdleState (Humanoid* humanoid)
            : HumanoidState(humanoid) { }
        virtual void Update () { }
        virtual void OnEnter ();
        virtual void OnExit () { }
        virtual void OnStruckEnemy () { }
        //virtual void OnStruck () { }
        virtual void OnAction (Action action) { }
    };

    class RunningState : public HumanoidState {
    public:
        explicit RunningState (Humanoid* humanoid);
        virtual void Update ();
        virtual void OnEnter ();
        virtual void OnExit () { }
        virtual void OnStruckEnemy () { }
        //virtual void OnStruck () { }
        virtual void OnAction (Action action) { }
    };

    class LadderState : public HumanoidState {
    public:
        explicit LadderState (Humanoid* humanoid);
        virtual void Update ();
        virtual void OnEnter ();
        virtual void OnExit ();
        virtual void OnStruckEnemy () { }
        //virtual void OnStruck () { }
        virtual void OnAction (Action action);
    private:
        class LadderTriggerTest : public ActionTriggerTest {
        public:
            LadderTriggerTest (LadderState* state)
                : m_state(state) { }
            virtual bool CanTransition ();
        private:
            LadderState* m_state;
        };

        Ladder* FindLadder ();

        Ladder* m_ladder;
    };

    class ContactListener : public Volt::EntityContactListener {
    public:
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
    bool m_superArmor;
    float m_runSpeed;
    float m_jumpTime;
    float m_jumpSpeed;

    /* States */
    void AddActionTrigger (Action action, ActionTriggerTest* test,
                           Humanoid::HumanoidState* state);
    void RemoveActionTriggers (Humanoid::HumanoidState* state);
    Volt::FSM* m_fsm;
    bool m_actionLock;
    struct Trigger {
        Action action;
        ActionTriggerTest* test;
        Humanoid::HumanoidState* changeToState;
    };
    vector<Trigger> m_actionTriggers;

    /* Animation */
    void PlayAnimTrack (const string& name);
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
