#pragma once

#include "Volt/Game/Scene.h"

class TitleScene : public Volt::Scene {
public:
    TitleScene ();
    virtual ~TitleScene ();

    virtual void Render ();
    virtual void Update ();

    virtual void OnBegin ();
    virtual void OnEnd ();
};
