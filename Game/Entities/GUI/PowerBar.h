#pragma once

#include "Core/Core.h"
#include "Volt/Game/Entity.h"
#include "GUIBar.h"

class PowerBar : public GUIBar {
public:
    PowerBar (float value, float speed) : GUIBar(value, speed) { };
    virtual ~PowerBar () { }

    virtual void Render ();

    virtual void OnAdded () { LOG(INFO) << "ADDED"; }
};
