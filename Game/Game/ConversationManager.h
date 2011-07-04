#pragma once

#include "Game/Core/Core.h"
#include "Game/Scenes/GameScene.h"
#include "Volt/Graphics/Input.h"
#include "Volt/Assets/AssetManager.h"
#include <queue>

class MessageBox;

class ConversationManager {
public:
    ConversationManager ();
    ~ConversationManager () { }

    void Update ();

    void OnKeyEvent (SDL_KeyboardEvent event);

    void ShowMessageBox (MessageBox* box);

private:
    friend class GameScene;
    GameScene* m_gameScene;

    Volt::FontAssetRef m_font;
    queue<MessageBox*> m_boxes;

    DISALLOW_COPY_AND_ASSIGN(ConversationManager);
};
