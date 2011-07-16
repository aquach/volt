#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"
#include "Game/Editor/EntityFactory.h"
#include "Volt/Assets/AssetManager.h"

struct DoodadBrush;

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

    void SetBrush (DoodadBrush* brush);
    void SetTint (Volt::Color tint) { m_tint = tint; }
    Volt::Color tint () const { return m_tint; }
    int brushId () const { return m_brushId; }

private:
    void CreatePhysicsBody ();

    DoodadBrush* m_brush;
    int m_brushId;
    Volt::Color m_tint;
    Volt::TextureAssetRef m_texture;

    DISALLOW_COPY_AND_ASSIGN(Doodad);
};
