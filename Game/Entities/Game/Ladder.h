#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"

class Ladder : public Entity {
public:
    Ladder ();
    virtual ~Ladder ();

    virtual Ladder* Clone () const;
    void CopyFrom (const Ladder* other);

    virtual void Update () { }
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    virtual void OnScaleChanged ();

    float topY () const { return m_transform.position.y +
                                 m_transform.scale.y * 0.5f; }
    float bottomY () const { return m_transform.position.y -
                                    m_transform.scale.y * 0.5f; }

private:
    void CreatePhysicsBody ();

    DISALLOW_COPY_AND_ASSIGN(Ladder);
};
