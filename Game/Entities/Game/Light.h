#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/Scene.h"
#include "Volt/Assets/AssetManager.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Game/Entity.h"

class LightManager;

struct LightStroke {
    float startRadius;
    float startAngle;
    float endRadius;
    float endAngle;
    Volt::Color color;
    float thickness;
};

/* A light that can cast colored light with attenuation and in a particular
 * direction. */
class Light : public Entity {
public:
    DECLARE_ENTITY_(Light);

    Light ();
    virtual ~Light ();

    virtual Light* Clone () const;
    void CopyFrom (const Light* other);

    virtual void OnAdded ();
    virtual void OnRemoved ();

    virtual void Update ();
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    virtual void OnScaleChanged ();
    virtual void GetProperties (vector<Property*>* properties);

    Volt::Color color () const { return m_color; }
    float maxDistance () const { return m_maxDistance; }
    float coneAngle () const { return m_coneAngle; }

    void InvalidateStaticMap ();

    Volt::BBox renderBounds () const;

    static const char* STATIC_MAP_FOLDER;

private:
    friend class LightManager;

    class LightSceneListener : public Volt::SceneListener {
    public:
        LightSceneListener (Light* l) : m_light(l) { }

        virtual void OnEntityRemoved (Volt::Entity* e);
    private:
        Light* m_light;
    };

    void CreatePhysicsBody ();
    void UpdateNearbyEntities ();
    void CreateStrokes ();
    vector<LightStroke> m_strokes;

    vector<Volt::Entity*> m_nearbyEntities;
    float m_nearbyEntitiesTimer;
    Volt::Color m_color;
    float m_maxDistance;
    float m_coneAngle;
    bool m_enabled;
    string m_name;
    bool m_static;
    Volt::TextureAssetRef m_staticMap;
    LightSceneListener* m_listener;
};
