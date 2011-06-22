#pragma once

#include "Volt/Game/Scene.h"

class Player;

class GameScene : public Volt::Scene {
public:
    GameScene ();
    virtual ~GameScene ();

    //virtual void Render ();
    virtual void Update ();

    virtual void OnBegin ();
    virtual void OnEnd ();
private:
    Player* m_player;
};