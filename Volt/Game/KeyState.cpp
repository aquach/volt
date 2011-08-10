#include "Volt/Game/KeyState.h"

namespace Volt {

bool KeyState::IsKeyPressed (SDLKey key) {
    return m_keyState[key - SDLK_FIRST];
}

void KeyState::Update (SDL_KeyboardEvent event) {
    SDLKey key = event.keysym.sym;
    m_keyState[key] = (event.type == SDL_KEYDOWN);
}

}
