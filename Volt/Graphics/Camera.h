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
    Entity* watchingEntity () const { return m_watchEntity; }

    Vector2 WorldToScreen (Vector2 v) const;
    Vector2 ScreenToWorld (Vector2 v) const;

    BBox worldBounds () const;

    Vector2 position () const { return transform.position; }

    Transform transform;
private:
    int m_backLayer;
    int m_frontLayer;
    Entity* m_watchEntity;
};

}
