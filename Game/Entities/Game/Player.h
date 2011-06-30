#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"
#include "Volt/Game/FSM.h"
#include "Volt/Graphics/Input.h"

namespace Volt {
    class Label;
};

class HealthBar;
class Ladder;
class PowerBar;
class Weapon;

class PlayerHitListener {
public:
    virtual void OnHit (Volt::Entity* agent, float damage) = 0;
};

/* TODO: Generalize Player to Humanoid Entity. */
class Player : public Volt::Entity {
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

    void EquipWeapon (Weapon* weapon);

    bool IsOnGround () const;

    void AddHitListener (PlayerHitListener* listener) {
        m_hitListeners.insert(listener);
    }
    void RemoveHitListener (PlayerHitListener* listener) {
        m_hitListeners.erase(listener);
    }

    Volt::Transform weaponTransform () const {
        return m_transform.Multiply(m_weaponTransform);
    }

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
    Volt::Transform m_weaponTransform;

    /* Player Attributes */
    void InvokeHitListeners (Volt::Entity* agent, float damage);
    float m_health;
    float m_maxHealth;
    float m_power;
    float m_maxPower;
    Weapon* m_weapon;
    set<PlayerHitListener*> m_hitListeners;

    /* GUI */
    bool m_debugDraw;
    Volt::Label* m_debugLabel;
    HealthBar* m_healthBar;
    PowerBar* m_powerBar;

    DISALLOW_COPY_AND_ASSIGN(Player);
};
