#pragma once

#include "Game/Core/Core.h"
#include <queue>
#include "Volt/Graphics/Input.h"
#include "Volt/Assets/AssetManager.h"
#include "Game/Scenes/GameScene.h"

class DialogBox;

class ConversationManager {
public:
    explicit ConversationManager (GameScene* scene);
    ~ConversationManager ();

    void Update ();

    void ShowDialogBox (DialogBox* box);

private:
    class InputListener : public GameInputListener {
    public:
        InputListener (ConversationManager* cm) : m_cm(cm) { }
        virtual ~InputListener () { }
        virtual bool OnKeyEvent (SDL_KeyboardEvent event);
    private:
        ConversationManager* m_cm;
    };
    bool OnKeyEvent (SDL_KeyboardEvent event);
    GameScene* m_gameScene;

    Volt::FontAssetRef m_font;
    queue<DialogBox*> m_boxes;
    InputListener* m_inputListener;

    DISALLOW_COPY_AND_ASSIGN(ConversationManager);
};
