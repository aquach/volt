#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/Scene.h"

class ConversationManager;
class DoodadManager;
class LevelManager;
class LightManager;
class DialogBox;
class ScriptConsole;

class GameScene : public Volt::Scene {
public:
    class GameInputListener {
    public:
        GameInputListener () : m_gameScene(NULL) { }
        virtual ~GameInputListener ();

        // Returns true if the listener handled the event.
        virtual bool OnKeyEvent (SDL_KeyboardEvent event) = 0;

        GameScene* gameScene () { return m_gameScene; }
    private:
        friend class GameScene;
        GameScene* m_gameScene;
    };

    GameScene ();
    virtual ~GameScene ();

    virtual void Update ();

    virtual void OnBegin ();
    virtual void OnEnd ();

    virtual void OnKeyEvent (SDL_KeyboardEvent event);

    void ShowDialogBox (DialogBox* box);

    void AddInputListener (GameInputListener* listener, int priority) {
        CHECK(listener->m_gameScene == NULL);
        m_inputListeners[priority].insert(listener);
        listener->m_gameScene = this;
    }
    void RemoveInputListener (GameInputListener* listener) {
        CHECK(listener->m_gameScene == this);
        bool removed = false;
        FOR_(InputListeners::iterator, i, m_inputListeners) {
            removed |= i->second.erase(listener) > 0;
        }
        if (removed)
            listener->m_gameScene = NULL;
    }

    void SetUpdateFreeze (bool enabled) { m_updateFreeze = enabled; }
    bool updateFrozen () const { return m_updateFreeze; }

private:
    LevelManager* m_levelManager;
    LightManager* m_lightManager;
    ConversationManager* m_conversationManager;
    DoodadManager* m_doodadManager;
    ScriptConsole* m_scriptConsole;
    bool m_updateFreeze;

    typedef map<int, set<GameInputListener*> > InputListeners;
    InputListeners m_inputListeners;
};
