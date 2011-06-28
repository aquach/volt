#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"
#include "GUIBar.h"

class HealthBar : public GUIBar {
public:
    HealthBar (float value, float speed) : GUIBar(value, speed) { };
    virtual ~HealthBar () { }

    virtual void Render ();
};
