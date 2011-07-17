#pragma once

#include "Game/Core/Core.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Game/Entity.h"

class Light : public Entity {
public:
    DECLARE_ENTITY_(Light);

    Light ();
    virtual ~Light ();

    virtual Light* Clone () const;
    void CopyFrom (const Light* other);

    virtual void Update ();
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    virtual void OnScaleChanged ();
    virtual void GetProperties (vector<Property*>* properties);

private:
    void CreatePhysicsBody ();

    Volt::Color m_color;
    float m_intensity;
};
