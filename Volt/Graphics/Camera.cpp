#include "Camera.h"
#include "Graphics.h"
#include "OpenGL.h"

namespace Volt {

Camera::Camera()
    : m_frontLayer(-100),
      m_backLayer(100) {
    transform.position = Graphics::GetScreenCenter();
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

}
