#include "Game/Entities/Game/Umbrella.h"
#include "Game/Graphics/Graphics.h"
#include "Game/Entities/Game/Player.h"

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
    Graphics::RenderQuad(1, 0.2);
    glPopMatrix();
}

void Umbrella::OnFire () {
}

void Umbrella::OnAlternateFire () {
}

void Umbrella::OnAlternateFire2 () {
}

