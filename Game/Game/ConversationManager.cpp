#include "Game/Game/ConversationManager.h"
#include "Game/Entities/Gui/DialogBox.h"

ConversationManager::ConversationManager (GameScene* scene)
    : m_gameScene(scene),
      m_inputListener(NULL) {
    m_font = G_AssetManager->GetFont("Fonts/Mido.ttf", 32.0);
    m_inputListener = new ConversationManager::InputListener(this);
    m_gameScene->AddInputListener(m_inputListener, 2);
}


ConversationManager::~ConversationManager () {
    while (!m_boxes.empty()) {
        DialogBox* box = m_boxes.front();
        if (box->scene() == NULL)
            delete box;
        m_boxes.pop();
    }
    m_gameScene->RemoveInputListener(m_inputListener);
}

bool ConversationManager::InputListener::OnKeyEvent (SDL_KeyboardEvent event) {
    return m_cm->OnKeyEvent(event);
}

bool ConversationManager::OnKeyEvent (SDL_KeyboardEvent event) {
    if (m_boxes.empty())
        return false;
    DialogBox* box = m_boxes.front();
    box->OnKeyEvent(event);
    return box->modal();
}

void ConversationManager::Update () {
    if (m_boxes.empty())
        return;

    DialogBox* box = m_boxes.front();
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
}


void ConversationManager::ShowDialogBox (DialogBox* box) {
    box->SetFont(m_font);
    m_boxes.push(box);
}
