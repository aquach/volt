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

    //virtual void BeginContact (Entity* other) { }
    //virtual void EndContact (Entity* other) { }
    //virtual bool PreSolve (Entity* other) { return true; }
    //virtual bool CanCollideWith (Entity* other) { return true; }

private:
    friend class SurfaceRayCallback;

    b2AABB GetGroundTestAABB ();
    void OnGroundCheck ();

    bool m_onGround;
    float m_jumpTimer;

    bool m_debugDraw;
};
