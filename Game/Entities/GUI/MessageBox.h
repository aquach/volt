#pragma once

#include "Game/Core/Core.h"
#include "Volt/Assets/AssetManager.h"
#include "Game/Game/Entity.h"
#include "Volt/Graphics/Input.h"

struct MessageBoxDef {
    MessageBoxDef ()
        : canSkip(true),
          modal(true),
          pauseDuration(2.0f) {
    }

    string text;
    bool modal;
    bool canSkip;

    // TODO: For ones that can't be skipped, how long to wait after finishing
    //       the display.
    // TODO: Dialog message boxes, like Yes/No.
    float pauseDuration;
};

class MessageBox : public Entity {
public:
    MessageBox (const MessageBoxDef& def);
    virtual ~MessageBox () { }

    virtual void Update ();
    virtual void Render ();

    bool IsFinished () const;
    bool HasCharactersRemaining () const;

    void OnKeyEvent (SDL_KeyboardEvent event);

    virtual void Load (const Json::Value& node) { }
    virtual void Save (Json::Value& node) const { }

private:
    friend class ConversationManager;

    void Skip ();

    Volt::FontAssetRef m_font;
    MessageBoxDef m_def;
    // Timer before displaying next character.
    float m_nextCharTimer;

    // Timer for blinking cursor to go to next.
    float m_nextCursorTimer;

    // Text to display.
    stringstream m_textStream;

    // Next character to be outputted from text.
    int m_currentCharacter;

    // Timer after dialog is finished to move on,
    // for unskippable boxes.
    float m_pauseTimer;

    // Set when box can be destroyed.
    bool m_finished;
};
