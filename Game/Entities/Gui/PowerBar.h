#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"
#include "Game/Entities/Gui/GuiBar.h"

class PowerBar : public GuiBar {
public:
    PowerBar (float value, float speed)
        : GuiBar(value, speed) {
        AddTag("PowerBar");
    };
    virtual ~PowerBar () { }

    virtual void Render ();

    virtual void Load (const Json::Value& node) { }
    virtual void Save (Json::Value& node) const { }
};
