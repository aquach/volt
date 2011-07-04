#include "Umbrella.h"
#include "Graphics/Graphics.h"
#include "Player.h"

Umbrella::Umbrella () {
}

Umbrella::~Umbrella () {
}

void Umbrella::Update () {
}

void Umbrella::Render () {
    glPushMatrix();
    Graphics::TransformMatrix(m_holder->weaponTransform());
    Graphics::SetColor(Volt::Color::RGB(235, 230, 230));
    Graphics::RenderCircle(0.8, 8);
    glPopMatrix();
}

void Umbrella::OnFire () {
}

void Umbrella::OnAlternateFire () {
}

void Umbrella::OnAlternateFire2 () {
}
