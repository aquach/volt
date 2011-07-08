#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"
#include "Game/Entities/GUI/GUIBar.h"

class HealthBar : public GUIBar {
public:
    HealthBar (float value, float speed)
        : GUIBar(value, speed) {
        AddTag("HealthBar");
    };
    virtual ~HealthBar () { }

    virtual void Render ();
};
