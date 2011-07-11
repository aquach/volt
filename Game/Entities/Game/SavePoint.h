#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"

class SavePoint : public Entity {
public:
    SavePoint ();
    virtual ~SavePoint ();

    virtual SavePoint* Clone () const;
    void CopyFrom (const SavePoint* other);

    virtual void Update () { }
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    virtual void OnScaleChanged ();

private:
    void CreatePhysicsBody ();

    DISALLOW_COPY_AND_ASSIGN(SavePoint);
};
