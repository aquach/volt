#pragma once

#include "Volt/Game/Scene.h"

class ConversationManager;
class DoodadManager;
class LevelManager;
class LightManager;
class DialogBox;
class Player;
class ScriptConsole;

class GameInputListener {
public:
    virtual ~GameInputListener () { }
    // Returns true if the listener handled the event.
    virtual bool OnKeyEvent (SDL_KeyboardEvent event) = 0;
};

class GameScene : public Volt::Scene {
public:
    GameScene ();
    virtual ~GameScene ();

    virtual void Update ();

    virtual void OnBegin ();
    virtual void OnEnd ();

    virtual void OnKeyEvent (SDL_KeyboardEvent event);

    void ShowDialogBox (DialogBox* box);
    void SetPlayerInputLock (bool lock) { m_playerInputLock = lock; }

    Player* player () { return m_player; }

    void AddInputListener (GameInputListener* listener, int priority) {
        m_inputListeners[priority].insert(listener);
    }
    void RemoveInputListener (GameInputListener* listener) {
        FOR_(InputListeners::iterator, i, m_inputListeners) {
            i->second.erase(listener);
        }
    }

private:
    Player* m_player;
    bool m_playerInputLock;

    LevelManager* m_levelManager;
    LightManager* m_lightManager;
    ConversationManager* m_conversationManager;
    DoodadManager* m_doodadManager;
    ScriptConsole* m_scriptConsole;

    typedef map<int, set<GameInputListener*> > InputListeners;
    InputListeners m_inputListeners;
};
