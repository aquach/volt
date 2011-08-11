#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"
#include "Game/Entities/Gui/GuiBar.h"

class HealthBar : public GuiBar {
public:
    HealthBar (float value, float speed)
        : GuiBar(value, speed) {
        AddTag("HealthBar");
    };
    virtual ~HealthBar () { }

    virtual void Render ();

    virtual void Load (const Json::Value& node) { }
    virtual void Save (Json::Value& node) const { }
};
