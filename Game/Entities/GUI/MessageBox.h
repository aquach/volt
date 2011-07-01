#pragma once

#include "Core/Core.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/Entity.h"


struct MessageBoxDef {
    MessageBoxDef ()
        : canSkip(true),
          modal(true) {
    }

    string text;
    bool modal;
    bool canSkip;
};

class MessageBox : public Volt::Entity {
public:
    MessageBox (const MessageBoxDef& def)
        : m_def(def) {
    };
    virtual ~MessageBox () { }

    virtual void Update ();
    virtual void Render ();

private:
    friend class ConversationManager;

    Volt::FontAssetRef m_font;
    MessageBoxDef m_def;
    float m_nextCharTimer;
    stringstream m_textStream;
};
