#pragma once

#include "Core/Core.h"

namespace Volt {

/* Manages where the camera is looking in the world. */
class Camera {
public:
    Camera ();

    void ApplyMatrix () const;

    void SetLayers (int backLayer, int frontLayer);
    int backLayer () const { return m_backLayer; }
    int frontLayer () const { return m_frontLayer; }

    Transform transform;
private:
    int m_backLayer;
    int m_frontLayer;
};

}
