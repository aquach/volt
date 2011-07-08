#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Game/Entity.h"

class Area : public Entity {
public:
    Area ();
    virtual ~Area ();

    virtual void Update ();
    virtual void Render ();

    //virtual void OnAdded () { }

    //virtual void BeginContact (Entity* other) { }
    //virtual void EndContact (Entity* other) { }
    //virtual bool PreSolve (Entity* other) { return true; }
    //virtual bool CanCollideWith (Entity* other) { return true; }
};
