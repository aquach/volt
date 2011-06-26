#include "Label.h"
#include "Graphics/Graphics.h"
#include "Graphics/OpenGL.h"

namespace Volt {

void Label::Render () {
    glPushMatrix();
    glLoadIdentity();

    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::SetColor(m_color);

    float x = m_x;
    float y = m_y;

    if (m_anchorX == ANCHOR_RIGHT)
        x -= m_font->GetTextWidth(m_text);
    else if (m_anchorX == ANCHOR_CENTER)
        x -= m_font->GetTextWidth(m_text) * 0.5f;

    if (m_anchorY == ANCHOR_TOP)
        y += m_font->GetTextHeight(m_text);
    else if (m_anchorY == ANCHOR_CENTER)
        y += m_font->GetTextHeight(m_text) * 0.5f;

    Graphics::RenderText(m_font, m_text, x, y);

    glPopMatrix();
}

}
