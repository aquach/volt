#pragma once

#include "Volt/Core/Core.h"

namespace Volt {

class Entity;

/* Manages where the camera is looking in the world. */
class Camera {
public:
    Camera ();

    void ApplyMatrix () const;

    void Update ();

    void SetLayers (int backLayer, int frontLayer);
    int backLayer () const { return m_backLayer; }
    int frontLayer () const { return m_frontLayer; }

    void WatchEntity (Entity* entity) {
        m_watchEntity = entity;
    }

    Vector2 WorldToScreen (Vector2 v) const;
    Vector2 ScreenToWorld (Vector2 v) const;

    BBox worldBounds () const;

    Transform transform;
private:
    int m_backLayer;
    int m_frontLayer;
    Entity* m_watchEntity;
};

}
