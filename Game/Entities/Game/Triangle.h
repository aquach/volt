#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"

/* A triangle of the terrain. */
class Triangle : public Entity {
public:
    Triangle ();
    virtual ~Triangle ();

    virtual void Update ();
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    int selectedVertex (Vector2 worldPoint) const;

    virtual void OnScaleChanged ();

    //virtual void OnAdded () { }

    //virtual void BeginContact (Entity* other) { }
    //virtual void EndContact (Entity* other) { }
    //virtual bool PreSolve (Entity* other) { return true; }
    //virtual bool CanCollideWith (Entity* other) { return true; }

private:
    void CreatePhysicsBody ();

    DISALLOW_COPY_AND_ASSIGN(Triangle);
};
