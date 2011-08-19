#pragma once

#include "Volt/Game/Scene.h"

class ConversationManager;
class DoodadManager;
class LevelManager;
class LightManager;
class DialogBox;
class Player;
class ScriptConsole;

class GameScene : public Volt::Scene {
public:
    GameScene ();
    virtual ~GameScene ();

    virtual void Update ();

    virtual void OnBegin ();
    virtual void OnEnd ();

    virtual void OnKeyEvent (SDL_KeyboardEvent event);
    //virtual void OnMouseButtonEvent (SDL_MouseButtonEvent event);
    //virtual void OnMouseMoveEvent (SDL_MouseMotionEvent event);

    void ShowDialogBox (DialogBox* box);
    void SetPlayerInputLock (bool lock) { m_inputLock = lock; }

    Player* player () { return m_player; }

private:
    Player* m_player;
    bool m_inputLock;

    LevelManager* m_levelManager;
    LightManager* m_lightManager;
    ConversationManager* m_conversationManager;
    DoodadManager* m_doodadManager;
    ScriptConsole* m_scriptConsole;
};
