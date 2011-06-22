#include "Camera.h"
#include "Game/Entity.h"
#include "Graphics.h"
#include "OpenGL.h"

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
    Graphics::Scale(transform.scale);
    Graphics::Translate(transform.position * -1.0f);
    glRotatef(transform.rotation, 0, 0, 1);
}

void Camera::SetLayers (int backLayer, int frontLayer) {
    m_frontLayer = frontLayer;
    m_backLayer = backLayer;
}

Vector2 Camera::WorldToScreen (Vector2 v) {
    v = v.Rotate(transform.rotation);
    v.x *= transform.scale.x;
    v.y *= transform.scale.y;
    return v - transform.position;
}

Vector2 Camera::ScreenToWorld (Vector2 v) {
    v += transform.position;
    v.x /= transform.scale.x;
    v.y /= transform.scale.y;
    v = v.Rotate(-transform.rotation);
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
