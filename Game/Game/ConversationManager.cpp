#include "ConversationManager.h"
#include "Entities/GUI/MessageBox.h"

ConversationManager::ConversationManager () {
    m_font = Volt::G_AssetManager->GetFont("Mido.ttf", 16.0);
}

/*
ConversationManager::~ConversationManager () {
}
*/

void ConversationManager::OnKeyEvent (SDL_KeyboardEvent event) {
    MessageBox* box = m_boxes.front();
    if (box->m_def.canSkip) {
        // Skip the box.
    }
}

void ConversationManager::Update () {
    m_gameScene->SetPlayerInputLock(false);
    
    if (m_boxes.empty())
        return;

    MessageBox* box = m_boxes.front();
    if (box->scene() == NULL)
        m_gameScene->Add(box);
    m_gameScene->SetPlayerInputLock(box->m_def.modal);
}


void ConversationManager::ShowMessageBox (MessageBox* box) {
    box->m_font = m_font;
    m_boxes.push(box);
}
