#pragma once

#include "Game/Core/Core.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Game/Entity.h"

class LightManager;

/* A light that can cast colored light with attenuation and in a particular
 * direction. */
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

    Volt::Color color () const { return m_color; }
    float maxDistance () const { return m_maxDistance; }
    float coneAngle () const { return m_coneAngle; }

private:
    friend class LightManager;
    void CreatePhysicsBody ();

    vector<Volt::Entity*> m_nearbyEntities;
    float m_nearbyEntitiesTimer;
    Volt::Color m_color;
    float m_maxDistance;
    float m_coneAngle;
};
