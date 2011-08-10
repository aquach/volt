#pragma once

#include "Game/Core/Core.h"
#include "Volt/Assets/AssetManager.h"
#include "Game/Game/Entity.h"
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
    float m_nextCharTimer;
    float m_nextCursorTimer;
    stringstream m_textStream;
    int m_currentCharacter;
    bool m_finished;
};
