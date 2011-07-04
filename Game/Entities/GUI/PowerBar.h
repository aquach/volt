#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/Entity.h"
#include "Game/Entities/GUI/GUIBar.h"

class PowerBar : public GUIBar {
public:
    PowerBar (float value, float speed)
        : GUIBar(value, speed) {
        AddTag("PowerBar");
    };
    virtual ~PowerBar () { }

    virtual void Render ();
};
