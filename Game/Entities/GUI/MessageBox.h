#pragma once

#include "Game/Core/Core.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/Entity.h"
#include "Volt/Graphics/Input.h"

struct MessageBoxDef {
    MessageBoxDef ()
        : canSkip(true),
          modal(true) {
    }

    string text;
    bool modal;
    bool canSkip;

    // TODO: For ones that can't be skipped, how long to wait after finishing
    //       the display.
    float pauseDuration;
};

class MessageBox : public Volt::Entity {
public:
    MessageBox (const MessageBoxDef& def);
    virtual ~MessageBox () { }

    virtual void Update ();
    virtual void Render ();

    bool IsFinished () const;
    bool HasCharactersRemaining () const;
    
    void OnKeyEvent (SDL_KeyboardEvent event);

private:
    friend class ConversationManager;

    void Skip ();

    Volt::FontAssetRef m_font;
    MessageBoxDef m_def;
    float m_nextCharTimer;
    stringstream m_textStream;
    int m_currentCharacter;
    bool m_finished;
};
