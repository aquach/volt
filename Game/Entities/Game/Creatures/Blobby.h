#pragma once

#include "Game/Core/Core.h"
#include "Game/Entities/Game/Creature.h"

class VelocityController;

class Blobby : public Creature {
public:
    Blobby ();
    virtual ~Blobby () { }

    virtual void Update ();
    virtual void Render ();

private:
    VelocityController* m_controller;
    float m_moveTimer;
    float m_pauseTimer;

    DISALLOW_COPY_AND_ASSIGN(Blobby);
};
