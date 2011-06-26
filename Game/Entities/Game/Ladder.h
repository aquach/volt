#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"
#include "Volt/Graphics/Input.h"

class Ladder : public Volt::Entity {
public:
    Ladder ();
    virtual ~Ladder ();

    virtual void Update () { }
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    //virtual void OnAdded () { }

    //virtual void BeginContact (Entity* other, b2Contact* contact);
    //virtual void EndContact (Entity* other, b2Contact* contact);
    //virtual bool PreSolve (Entity* other) { return true; }
    //virtual bool CanCollideWith (Entity* other) { return true; }
};
