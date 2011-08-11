#pragma once

#include "Game/Core/Core.h"
#include "Game/Entities/Gui/DialogBox.h"

struct MessageBoxDef {
    MessageBoxDef ()
        : canSkip(true),
          modal(true),
          pauseDuration(2.0f) {
    }

    string text;
    bool modal;
    bool canSkip;

    // For ones that can't be skipped, how long to wait after finishing
    // the display.
    float pauseDuration;
};

class MessageBox : public DialogBox {
public:
    MessageBox (const MessageBoxDef& def);
    virtual ~MessageBox () { }

    virtual void Update ();
    virtual void Render ();
    virtual void OnAdded ();
    virtual bool modal () const { return m_def.modal; }

    virtual void OnKeyEvent (SDL_KeyboardEvent event);

private:
    void Skip ();

    bool HasCharactersRemaining () const;
    int textLineMaxWidth ();

    MessageBoxDef m_def;

    // Timer before displaying next character.
    float m_nextCharTimer;

    // Timer for blinking cursor to go to next message box.
    float m_nextCursorTimer;

    // Text to display.
    stringstream m_textStream;

    // Next character to be outputted from text.
    int m_currentCharacter;

    // Timer after dialog is finished to move on,
    // for unskippable boxes.
    float m_pauseTimer;
};
