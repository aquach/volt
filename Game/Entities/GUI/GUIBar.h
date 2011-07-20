#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"

class Player;

class GUIBar : public Entity {
public:
    GUIBar (float value, float speed)
        : m_target(value),
          m_currentValue(value),
          m_speed(speed) { };
    virtual ~GUIBar () { }

    virtual void Update ();
    virtual void Render () = 0;

    void Set (float value) { m_target = value; m_currentValue = value; }
    void AnimateTo (float value) { m_target = value; }

    float value () const { return m_currentValue; }
    float targetValue () const { return m_target; }

    virtual void Load (const Json::Value& node) { }
    virtual void Save (Json::Value& node) const { }

protected:
    float m_target;
    float m_currentValue;
    float m_speed;
};
