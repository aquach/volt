#pragma once

#include "Game/Core/Core.h"

class Property {
public:
    Property (string name)
        : m_name(name) { }
    virtual void Read (string input) = 0;
    virtual void Write (string* out) = 0;

    string name () const { return m_name; }
protected:
    string m_name;
};
