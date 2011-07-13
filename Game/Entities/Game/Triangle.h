#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"
#include "Game/Editor/EntityFactory.h"

/* A triangle of the terrain. */
class Triangle : public Entity {
public:
    Triangle ();
    virtual ~Triangle ();

    virtual Triangle* Clone () const;
    void CopyFrom (const Triangle* other);

    virtual void Update ();
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    int selectedVertex (Vector2 worldPoint) const;

    virtual void OnScaleChanged ();

private:
    void CreatePhysicsBody ();

    DECLARE_ENTITY_(Triangle);

    DISALLOW_COPY_AND_ASSIGN(Triangle);
};
