#pragma once

#include "Core/Core.h"
#include "Graphics/Input.h"
#include <map>

namespace Volt {

class KeyBindings {
public:
    typedef map<string, SDLKey> Bindings;

    KeyBindings () { }
    ~KeyBindings () { }

    static void Register (KeyBindings* manager) { instance = manager; }
    static KeyBindings* Instance () {
        CHECK_NOTNULL(instance);
        return instance;
    }

    SDLKey Get (string name) const {
        Bindings::const_iterator i = m_bindings.find(name);
        if (i == m_bindings.end())
            return (SDLKey)-1;
        return i->second;
    }

    void Set (string name, SDLKey key) {
        m_bindings[name] = key;
    }

    void GetBindings (Bindings& bindings) const {
        bindings.insert(m_bindings.begin(), m_bindings.end());
    }

private:
    Bindings m_bindings;

    static KeyBindings* instance;

    DISALLOW_COPY_AND_ASSIGN(KeyBindings);
};

#define G_KeyBindings KeyBindings::Instance()

}
