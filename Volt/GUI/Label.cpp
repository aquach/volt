#include "Label.h"
#include "Graphics/Graphics.h"

namespace Volt {

void Label::Render () {
    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::SetColor(m_color);

    if (m_centered) {
        Graphics::RenderTextCentered(m_font, m_text, m_x, m_y);
    } else {
        Graphics::RenderText(m_font, m_text, m_x, m_y);
    }
}

}
