#include "Volt/Graphics/Camera.h"
#include "Volt/Game/Entity.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"

namespace Volt {

const float TRACKING_SPEED = 0.1f;
const float SLACK = 10.0f;

Camera::Camera ()
    : m_frontLayer(-100),
      m_backLayer(100),
      m_watchEntity(NULL) {
}

void Camera::ApplyMatrix () const {
    glMatrixMode(GL_MODELVIEW);
    Graphics::DefaultMatrix();

    // Opposite from Graphics::TransformMatrix because that's Obj to World
    // and transform in this case is World to Obj (Screen).
    Graphics::Scale(transform.scale);
    glRotatef(transform.rotation, 0, 0, 1);
    Graphics::Translate(transform.position * -1.0f);
}

void Camera::SetLayers (int backLayer, int frontLayer) {
    m_frontLayer = frontLayer;
    m_backLayer = backLayer;
}

Vector2 Camera::WorldToScreen (Vector2 v) {
    v -= transform.position;
    v = v.Rotate(transform.rotation);
    v.x *= transform.scale.x;
    v.y *= transform.scale.y;
    v += Graphics::GetScreenCenter();
    return v;
}

Vector2 Camera::ScreenToWorld (Vector2 v) {
    v -= Graphics::GetScreenCenter();
    v.x /= transform.scale.x;
    v.y /= transform.scale.y;
    v = v.Rotate(-transform.rotation);
    v += transform.position;

    return v;
}

void Camera::Update () {
    if (m_watchEntity == NULL)
        return;

    Vector2 entityScreenPos = WorldToScreen(m_watchEntity->position());
    if (entityScreenPos.LengthSquared() > SLACK) {
        Vector2 dir = m_watchEntity->position() - transform.position;
        transform.position += dir * TRACKING_SPEED;
    }
}

}
