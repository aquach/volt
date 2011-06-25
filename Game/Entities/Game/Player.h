#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"
#include "Volt/Graphics/Input.h"

class Player : public Volt::Entity {
public:
    Player ();
    virtual ~Player ();

    virtual void Update ();
    virtual void Render ();

    void OnKeyEvent (SDL_KeyboardEvent event);

    //virtual void OnAdded () { }

    virtual void BeginContact (Entity* other, b2Contact* contact);
    virtual void EndContact (Entity* other, b2Contact* contact);
    //virtual bool PreSolve (Entity* other) { return true; }
    //virtual bool CanCollideWith (Entity* other) { return true; }

    bool IsOnGround () const;

private:
    b2Body* m_sideContacts[4];
    float m_jumpTimer;

    bool m_debugDraw;
};
