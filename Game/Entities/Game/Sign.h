#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"

class Sign : public Entity {
public:
    Sign ();
    virtual ~Sign ();

    virtual Sign* Clone () const;
    void CopyFrom (const Sign* other);

    virtual void Update () { }
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    virtual void OnAccessed (Entity* accessor);

    virtual void OnScaleChanged ();

private:
    void CreatePhysicsBody ();

    DISALLOW_COPY_AND_ASSIGN(Sign);
};
