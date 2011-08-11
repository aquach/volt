#pragma once

#include "Game/Core/Core.h"
#include "Volt/Assets/AssetManager.h"
#include "Game/Game/Entity.h"
#include "Volt/Graphics/Input.h"

/* A DialogBox managed by the ConversationManager. DialogBoxes are Entities
 * that receive KeyEvents and eventually set a finished flag, indicating to
 * the manager that its operation is complete and it can be destroyed.
 */
class DialogBox : public Entity {
public:
    DialogBox () : m_finished(false) { }
    virtual ~DialogBox () { }

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

protected:
    Volt::FontAssetRef m_font;

    // Set when box can be destroyed.
    bool m_finished;

private:
    DISALLOW_COPY_AND_ASSIGN(DialogBox);
};
