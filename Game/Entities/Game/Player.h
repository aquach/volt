#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"

class Player : public Volt::Entity {
public:
    Player ();
    virtual ~Player ();

    virtual void Update ();
    virtual void Render ();

    //virtual void OnAdded () { }

    //virtual void BeginContact (Entity* other) { }
    //virtual void EndContact (Entity* other) { }
    //virtual bool PreSolve (Entity* other) { return true; }
    //virtual bool CanCollideWith (Entity* other) { return true; }
};
