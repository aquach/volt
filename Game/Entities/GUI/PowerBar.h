#pragma once

#include "Game/Core/Core.h"
#include "Game/Game/Entity.h"
#include "Game/Entities/GUI/GUIBar.h"

class PowerBar : public GUIBar {
public:
    PowerBar (float value, float speed)
        : GUIBar(value, speed) {
        AddTag("PowerBar");
    };
    virtual ~PowerBar () { }

    virtual void Render ();

    virtual void Load (const Json::Value& node) { }
    virtual void Save (Json::Value& node) const { }
};
