#pragma once

#include "Volt/Game/Scene.h"

class ConversationManager;
class DoodadManager;
class LevelManager;
class MessageBox;
class Player;

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

    void ShowMessageBox (MessageBox* box);
    void SetPlayerInputLock (bool lock);

private:
    Player* m_player;

    LevelManager* m_levelManager;
    ConversationManager* m_conversationManager;
    DoodadManager* m_doodadManager;
};
