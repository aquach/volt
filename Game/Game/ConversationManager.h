#pragma once

#include "Game/Core/Core.h"
#include "Volt/Graphics/Input.h"
#include "Volt/Assets/AssetManager.h"
#include <queue>

class GameScene;
class DialogBox;

class ConversationManager {
public:
    explicit ConversationManager (GameScene* scene);
    ~ConversationManager ();

    void Update ();

    void OnKeyEvent (SDL_KeyboardEvent event);

    void ShowDialogBox (DialogBox* box);

private:
    GameScene* m_gameScene;

    Volt::FontAssetRef m_font;
    queue<DialogBox*> m_boxes;

    DISALLOW_COPY_AND_ASSIGN(ConversationManager);
};
