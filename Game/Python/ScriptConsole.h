#pragma once

#include "Game/Core/Core.h"
#include <deque>
#include "Volt/Graphics/Input.h"
#include "Volt/Game/Entity.h"
#include "Volt/Assets/AssetManager.h"
#include "Game/Scenes/GameScene.h"

namespace Volt {
    class Label;
}

class ScriptConsole : public Volt::Entity {
public:
    explicit ScriptConsole ();
    ~ScriptConsole ();

    virtual void OnAdded ();
    virtual void OnRemoved ();

    virtual void Update ();
    virtual void Render ();

private:
    class InputListener : public GameScene::GameInputListener {
    public:
        InputListener (ScriptConsole* sl) : m_sl(sl) { }
        virtual ~InputListener () { }
        virtual bool OnKeyEvent (SDL_KeyboardEvent event);
    private:
        ScriptConsole* m_sl;
    };
    bool OnKeyEvent (SDL_KeyboardEvent event);

    deque<string> m_history;
    int m_historyLocation;
    string m_currentText;
    int m_cursor;
    float m_blinkTimer;
    bool m_visible;
    Volt::Label* m_label;
    GameScene::GameInputListener* m_inputListener;
};
