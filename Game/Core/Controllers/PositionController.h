#pragma once

#include "Game/Core/Core.h"

class Entity;
class PositionController {
public:
    PositionController (Entity* entity, Vector2 targetPos)
        : m_entity(entity),
          m_targetPos(targetPos),
          m_p(2),
          m_d(0.2),
          m_maxSpeed(3) {
    }

    void Update ();

    void SetTargetPos (Vector2 pos) { m_targetPos = pos; }
    Vector2 targetPos () const { return m_targetPos; }

    void SetP (float p) { m_p = p; }
    float p () const { return m_p; }

    void SetD (float d) { m_d = d; }
    float d () const { return m_d; }
    void SetMaxSpeed (float s) { m_maxSpeed = s; }
    float maxSpeed () const { return m_maxSpeed; }

private:
    Entity* m_entity;
    Vector2 m_targetPos;
    float m_p;
    float m_d;
    float m_maxSpeed;
};
