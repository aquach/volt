#include "MessageBox.h"
#include "Graphics/Graphics.h"
#include "Volt/Graphics/Window.h"

const float HEIGHT = 250;
const float MARGIN = 40;
const float TEXT_MARGIN = 50;

void MessageBox::Update () {
}

void MessageBox::Render () {
    glPushMatrix();
    glLoadIdentity();

    float windowWidth = Volt::G_Window->width();
    float windowHeight = Volt::G_Window->height();
    Volt::BBox box(
        Vector2(0, windowHeight - HEIGHT),
        Vector2(windowWidth, windowHeight)
    );
    box = box.Expand(-MARGIN);
    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::SetColor(Volt::Color::RGBA(150, 150, 200, 70));
    Graphics::Translate(box.center());
    Graphics::RenderQuad(box.extents().x, box.extents().y);
    glLoadIdentity();
    
    Volt::BBox textBox = box.Expand(-TEXT_MARGIN);
    Graphics::SetColor(Volt::Color::RGB(0, 0, 0));
    Graphics::RenderText(m_font, m_def.text, textBox.min.x, textBox.min.y);

    glPopMatrix();
}
