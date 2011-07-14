#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"
#include "Game/Editor/EntityFactory.h"
#include "Volt/Assets/AssetManager.h"

struct DoodadBrush {
    Vector2 m_textureCoords[4];
    Volt::TextureAssetRef m_texture;
    Vector2 m_size;
};

/* A stamp-like tile thing to be arranged in the world for decorative purposes.
 */
class Doodad : public Entity {
public:
    DECLARE_ENTITY_(Doodad);

    Doodad ();
    virtual ~Doodad ();

    virtual Doodad* Clone () const;
    void CopyFrom (const Doodad* other);

    virtual void Update ();
    virtual void Render ();

    virtual void Load (const Json::Value& node);
    virtual void Save (Json::Value& node) const;

    virtual void OnScaleChanged ();
    virtual void GetProperties (vector<Property*>* properties);

private:
    void CreatePhysicsBody ();

    DoodadBrush* m_brush;
    Volt::Color m_tint;

    DISALLOW_COPY_AND_ASSIGN(Doodad);
};
