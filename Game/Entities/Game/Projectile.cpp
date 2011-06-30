#include "Projectile.h"
#include "Core/Controllers/ProjectileController.h"
#include "Graphics/Graphics.h"

Projectile::Projectile ()
    : m_controller(NULL) {
    m_transform.scale.x = 2;
    m_transform.position = Vector2(5, 5);

    ProjectileControllerDef pDef;
    m_controller = new ProjectileController(this, Vector2(0, 0), pDef);

    b2BodyDef def;
    def.type = b2_kinematicBody;
    m_body = CreateBody(def);
}

Projectile::~Projectile () {
}

void Projectile::Update () {
    UpdatePhysics();
    m_controller->Update();
}

void Projectile::Render () {
    Graphics::SetColor(Volt::Color::RGB(50, 0, 100));
    glPushMatrix();
    Graphics::TransformMatrix(m_transform);
    Graphics::RenderTriangle(1, 1);
    glPopMatrix();
}
