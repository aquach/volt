#pragma once

#include "Core/Core.h"
#include "Creature.h"
#include "Volt/Game/FSM.h"
#include "Volt/Graphics/Input.h"

namespace Volt {
    class Label;
};

class Ladder;

class Player : public Creature {
public:
    Player ();
    virtual ~Player () { }

    virtual void Update ();
    virtual void Render ();

    void OnKeyEvent (SDL_KeyboardEvent event);

    virtual void OnAdded ();
    virtual void OnRemoved ();

    virtual void BeginContact (Entity* other, b2Contact* contact);
    virtual void EndContact (Entity* other, b2Contact* contact);
    //virtual bool PreSolve (Entity* other) { return true; }
    //virtual bool CanCollideWith (Entity* other) { return true; }

    bool IsOnGround () const;

private:
    class PlayerState : public Volt::FSMState {
    public:
        explicit PlayerState (Player* player)
            : m_p(player) { }
        Player* m_p;

        virtual void OnKeyEvent (SDL_KeyboardEvent event) { }
    private:
        DISALLOW_COPY_AND_ASSIGN(PlayerState);
    };
    class NormalState : public PlayerState {
    public:
        explicit NormalState (Player* player)
            : PlayerState(player) { }
        virtual void Update ();
        virtual void OnEnter ();
        virtual void OnExit ();

        virtual void OnKeyEvent (SDL_KeyboardEvent event);
    };
    class LadderState : public PlayerState {
    public:
        explicit LadderState (Player* player)
            : PlayerState(player) { }
        virtual void Update ();
        virtual void OnEnter ();
        virtual void OnExit ();

        virtual void OnKeyEvent (SDL_KeyboardEvent event);
    };

    /* Physics */
    void UpdateJump ();

    Volt::FSM* m_fsm;
    b2Body* m_sideContacts[4];
    float m_jumpTimer;
    Ladder* m_ladder;

    /* Animation */

    /* Player Attributes */

    /* GUI */
    bool m_debugDraw;
    Volt::Label* m_debugLabel;
    HealthBar* m_healthBar;
    PowerBar* m_powerBar;

    DISALLOW_COPY_AND_ASSIGN(Player);
};
