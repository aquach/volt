#pragma once

#include "Volt/Game/Scene.h"

class Player;
class LevelManager;

class GameScene : public Volt::Scene {
public:
    GameScene ();
    virtual ~GameScene ();

    //virtual void Render ();
    virtual void Update ();

    virtual void OnBegin ();
    virtual void OnEnd ();

    virtual void OnKeyEvent (SDL_KeyboardEvent event);
    //virtual void OnMouseButtonEvent (SDL_MouseButtonEvent event);
    //virtual void OnMouseMoveEvent (SDL_MouseMotionEvent event);

private:
    Player* m_player;
    LevelManager* m_levelManager;
};
