#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"

/* A triangle of the terrain. */
class Triangle : public Volt::Entity {
public:
    Triangle ();
    virtual ~Triangle ();

    virtual void Update ();
    virtual void Render ();

    void Load (const Json::Value& node);
    void Save (Json::Value& node) const;

    //virtual void OnAdded () { }

    //virtual void BeginContact (Entity* other) { }
    //virtual void EndContact (Entity* other) { }
    //virtual bool PreSolve (Entity* other) { return true; }
    //virtual bool CanCollideWith (Entity* other) { return true; }
};
