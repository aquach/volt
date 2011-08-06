#include "Game/Game/ConversationManager.h"
#include "Game/Entities/GUI/MessageBox.h"
#include "Game/Scenes/GameScene.h"

ConversationManager::ConversationManager (GameScene* scene)
    : m_gameScene(scene) {
    m_font = G_AssetManager->GetFont("Fonts/Mido.ttf", 32.0);
}


ConversationManager::~ConversationManager () {
    while (!m_boxes.empty()) {
        MessageBox* box = m_boxes.front();
        if (box->scene() == NULL)
            delete box;
        m_boxes.pop();
    }
}

void ConversationManager::OnKeyEvent (SDL_KeyboardEvent event) {
    if (m_boxes.empty())
        return;
    MessageBox* box = m_boxes.front();
    box->OnKeyEvent(event);
}

void ConversationManager::Update () {
    // TODO: Fix player input. Make keyboard niput something more sensible.
    m_gameScene->SetPlayerInputLock(false);

    if (m_boxes.empty())
        return;

    MessageBox* box = m_boxes.front();
    if (box->scene() == NULL)
        m_gameScene->Add(box);

    if (box->IsFinished()) {
        m_gameScene->Remove(box);
        m_boxes.pop();
        box = NULL;
        if (!m_boxes.empty()) {
            box = m_boxes.front();
            m_gameScene->Add(box);
        }
    }

    if (box != NULL) {
        m_gameScene->SetPlayerInputLock(box->m_def.modal);
    } else {
        /* Keep it locked until the next update so the player can't see the
         * key that ended this MessageBox. */
        m_gameScene->SetPlayerInputLock(true);
    }
}


void ConversationManager::ShowMessageBox (MessageBox* box) {
    box->m_font = m_font;
    m_boxes.push(box);
}
