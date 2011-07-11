#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"

class Light : public Entity {
public:
    Light ();
    virtual ~Light ();

    virtual void Update ();
    virtual void Render ();

    //virtual void OnAdded () { }

    //virtual void BeginContact (Entity* other) { }
    //virtual void EndContact (Entity* other) { }
    //virtual bool PreSolve (Entity* other) { return true; }
    //virtual bool CanCollideWith (Entity* other) { return true; }
};
