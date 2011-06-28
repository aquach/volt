#pragma once

#include "Core/Core.h"
#include "Assets/AssetManager.h"
#include "Game/Entity.h"

namespace Volt {

class Label : public Entity {
public:
    enum AnchorPosition {
        ANCHOR_CENTER = 0,
        ANCHOR_LEFT = 1,
        ANCHOR_RIGHT = 2,
        ANCHOR_TOP = 1,
        ANCHOR_BOTTOM = 2
    };

    Label (FontAssetRef font, float x, float y)
        : m_font(font),
          m_x(x),
          m_y(y),
          m_anchorX(ANCHOR_LEFT),
          m_anchorY(ANCHOR_TOP),
          m_color(Color::white) {
    }

    AnchorPosition anchorX () const { return m_anchorX; }
    AnchorPosition anchorY () const { return m_anchorY; }
    void SetAnchorX (AnchorPosition p) { m_anchorX = p; }
    void SetAnchorY (AnchorPosition p) { m_anchorY = p; }
    void SetAnchor (AnchorPosition x, AnchorPosition y) {
        m_anchorX = x;
        m_anchorY = y;
    }

    string text () const { return m_text; }
    void SetText (string text) { m_text = text; }

    Color color () const { return m_color; }
    void SetColor (Color c) { m_color = c; }

    virtual void Update () {}
    virtual void Render ();

private:
    FontAssetRef m_font;
    Color m_color;
    string m_text;
    float m_x;
    float m_y;
    AnchorPosition m_anchorX;
    AnchorPosition m_anchorY;
};

}
