#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"
#include "Game/Editor/EntityFactory.h"
#include "Volt/Assets/AssetManager.h"

/* A triangle of the terrain. */
class Triangle : public Entity {
public:
	static const int NUM_VERTS = 3;

    DECLARE_ENTITY_(Triangle);

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

    virtual void GetProperties (vector<Property*>* properties);

private:
    void CreatePhysicsBody ();
    Volt::Color m_vertexColors[NUM_VERTS];
    Vector2 m_textureCoords[NUM_VERTS];
    Volt::TextureAssetRef m_texture;

    DISALLOW_COPY_AND_ASSIGN(Triangle);
};
