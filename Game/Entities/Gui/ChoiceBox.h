#pragma once

#include "Game/Core/Core.h"
#include "Game/Entities/Gui/DialogBox.h"

struct ChoiceBoxDef {
    ChoiceBoxDef ()  {
    }

    string text;
    vector<string> choices;
};

class ChoiceBox : public DialogBox {
public:
    ChoiceBox (const ChoiceBoxDef& def);
    virtual ~ChoiceBox () { }

    virtual void Update ();
    virtual void Render ();
    virtual bool modal () const { return true; }
    virtual void OnAdded ();

    int choice () const { return m_choice; }

    virtual void OnKeyEvent (SDL_KeyboardEvent event);

private:
    void Skip ();

    bool HasCharactersRemaining () const;
    int textLineMaxWidth ();

    ChoiceBoxDef m_def;

    // Timer before displaying next character.
    float m_nextCharTimer;

    // Text to display.
    stringstream m_textStream;

    // Next character to be outputted from text.
    int m_currentCharacter;

    int m_choice;
};

