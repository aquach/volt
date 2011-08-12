#pragma once

#include "Game/Core/Core.h"
#include "Volt/Assets/AssetManager.h"
#include "Game/Game/Entity.h"
#include "Volt/Graphics/Input.h"

class DialogListener {
public:
    virtual void OnDialogFinished () = 0;
};

/* A DialogBox managed by the ConversationManager. DialogBoxes are Entities
 * that receive KeyEvents and eventually set a finished flag, indicating to
 * the manager that its operation is complete and it can be destroyed.
 */
class DialogBox : public Entity {
public:
    DialogBox () : m_finished(false) { }
    virtual ~DialogBox () {
        FOR_(set<DialogListener*>::iterator, i, m_dialogListeners)
            delete *i;
    }

    bool IsFinished () const { return m_finished; }

    // Whether or not the dialog blocks KeyEvents to the player.
    virtual bool modal () const = 0;

    virtual void Update () = 0;
    virtual void Render () = 0;

    void SetFont (Volt::FontAssetRef font) { m_font = font; }

    virtual void OnKeyEvent (SDL_KeyboardEvent event) = 0;

    virtual void Load (const Json::Value& node) { }
    virtual void Save (Json::Value& node) const { }

    /* Removes newlines, collapses multiple spaces to one space,
     * and word-wraps. */
    static void ProcessText (string& text, Volt::FontAssetRef font,
                             float maxWidth);

    void AddDialogListener (DialogListener* listener) {
        m_dialogListeners.insert(listener);
    }
    void RemoveDialogListener (DialogListener* listener) {
        m_dialogListeners.erase(listener);
    }

protected:
    // Call this in child classes when the DialogBox is finished.
    void OnFinished () {
        FOR_(set<DialogListener*>::iterator, i, m_dialogListeners)
            (*i)->OnDialogFinished();
        m_finished = true;
    }

    Volt::FontAssetRef m_font;

private:
    // Set when box can be destroyed.
    bool m_finished;

    set<DialogListener*> m_dialogListeners;
    DISALLOW_COPY_AND_ASSIGN(DialogBox);
};
