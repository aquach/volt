#include "PowerBar.h"
#include "Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"

const float WIDTH = 400;
const float HEIGHT = 20;
const float X = 10;
const float Y = 50;

void PowerBar::Render () {
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(X, Y + HEIGHT / 2, 0);

    Graphics::SetBlend(Graphics::BLEND_NONE);
    Graphics::SetColor(Volt::Color::RGB(255, 238, 0));
    float amount = m_target * WIDTH;
    glTranslatef(amount / 2, 0, 0);
    Graphics::RenderQuad(amount, HEIGHT);

    Graphics::SetColor(Volt::Color::RGB(128, 119, 0));
    float targetAmount = abs(m_target - m_currentValue) * WIDTH;
    glTranslatef(amount / 2 + targetAmount / 2, 0, 0);
    Graphics::RenderQuad(targetAmount, HEIGHT);

    Graphics::SetColor(Volt::Color::RGB(50, 50, 50));
    float missingAmount = WIDTH - amount - targetAmount;
    glTranslatef(missingAmount / 2 + targetAmount / 2, 0, 0);
    Graphics::RenderQuad(missingAmount, HEIGHT);

    glPopMatrix();
}
