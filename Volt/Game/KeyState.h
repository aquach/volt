#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Graphics/Input.h"

namespace Volt {

/* Manages a local KeyState. Used by certain classes who only want
 * to know if the key is pressed through an event that they handled,
 * instead of the overall state of the key through SDLWindow.
 */
class KeyState {
public:
    KeyState () {
        memset(m_keyState, 0, sizeof(bool) * (SDLK_LAST - SDLK_FIRST));
    }
    ~KeyState () { }

    bool IsKeyPressed (SDLKey key);
    void Update (SDL_KeyboardEvent event);

private:
    bool m_keyState[SDLK_LAST - SDLK_FIRST];
};

}
