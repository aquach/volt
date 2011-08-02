#pragma once

#include "Game/Core/Core.h"
#include <deque>
#include "Volt/Graphics/Input.h"
#include "Volt/Assets/AssetManager.h"

namespace Volt {
    class Label;
    class Scene;
}

class ScriptConsole {
public:
    explicit ScriptConsole (Volt::Scene* scene);
    ~ScriptConsole ();

    void Render ();

    bool OnKeyEvent (SDL_KeyboardEvent event);

private:
    Volt::Scene* m_scene;
    deque<string> m_history;
    int m_historyLocation;
    string m_currentText;
    bool m_visible;
    Volt::Label* m_label;
};
