#pragma once

#include "Game/Core/Core.h"

class Property {
public:
    Property (string name)
        : m_name(name) { }
    virtual void Save (string input) = 0;
    virtual void Load (string* out) const = 0;

    string name () const { return m_name; }
protected:
    string m_name;
};

class Vector2Property : public Property {
public:
    Vector2Property (string name, Vector2* v)
        : Property(name), m_v(v) { }
    virtual void Save (string input);
    virtual void Load (string* out) const;
private:
    Vector2* m_v;
};

class FloatProperty : public Property {
public:
    FloatProperty (string name, float* num)
        : Property(name), m_num(num) { }
    virtual void Save (string input);
    virtual void Load (string* out) const;
private:
    float* m_num;
};

class IntProperty : public Property {
public:
    IntProperty (string name, int* num)
        : Property(name), m_num(num) { }
    virtual void Save (string input);
    virtual void Load (string* out) const;
private:
    int* m_num;
};
