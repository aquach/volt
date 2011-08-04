#pragma once

#include "Game/Core/Core.h"

class Entity;

class Property {
public:
    Property (const string& name)
        : m_name(name) { }
    virtual void Save (const string& input) = 0;
    virtual void Load (string* out) const = 0;

    virtual bool editable () const { return true; }

    virtual bool checkable () const { return false; }
    virtual bool checked () const { return false; }
    virtual void setChecked (bool b) { }

    const string& name () const { return m_name; }
protected:
    string m_name;
};

class Vector2Property : public Property {
public:
    Vector2Property (const string& name, Vector2* v)
        : Property(name), m_v(v) { }
    virtual void Save (const string& input);
    virtual void Load (string* out) const;
private:
    Vector2* m_v;
};

class FloatProperty : public Property {
public:
    FloatProperty (const string& name, float* num)
        : Property(name), m_num(num) { }
    virtual void Save (const string& input);
    virtual void Load (string* out) const;
private:
    float* m_num;
};

class IntProperty : public Property {
public:
    IntProperty (const string& name, int* num)
        : Property(name), m_num(num) { }
    virtual void Save (const string& input);
    virtual void Load (string* out) const;
private:
    int* m_num;
};

class BoolProperty : public Property {
public:
    BoolProperty (const string& name, bool* b)
        : Property(name), m_b(b) { }
    virtual void Save (const string& input) { }
    virtual void Load (string* out) const { }

    virtual bool checkable () const { return true; }
    virtual bool checked () const { return *m_b; }
    virtual void setChecked (bool b) { *m_b = b; }
private:
    bool* m_b;
};

class ColorProperty : public Property {
public:
    ColorProperty (const string& name, Volt::Color* color)
        : Property(name), m_color(color) { }
    virtual void Save (const string& input);
    virtual void Load (string* out) const;
    void Set (Volt::Color c) { *m_color = c; }
    Volt::Color value () const { return *m_color; }
private:
    Volt::Color* m_color;
};

class StringProperty : public Property {
public:
    StringProperty (const string& name, string* str)
        : Property(name), m_str(str) { }
    virtual void Save (const string& input);
    virtual void Load (string* out) const;
private:
    string* m_str;
};

class EntityLayerProperty : public Property {
public:
    EntityLayerProperty (const string& name, Entity* e)
        : Property(name), m_entity(e) { }
    virtual void Save (const string& input);
    virtual void Load (string* out) const;

    virtual bool editable () const { return true; }
private:
    Entity* m_entity;
};

class EntityPositionProperty : public Property {
public:
    EntityPositionProperty (const string& name, Entity* e)
        : Property(name), m_entity(e) { }
    virtual void Save (const string& input);
    virtual void Load (string* out) const;

    virtual bool editable () const { return true; }
private:
    Entity* m_entity;
};

class EntityRotationProperty : public Property {
public:
    EntityRotationProperty (const string& name, Entity* e)
        : Property(name), m_entity(e) { }
    virtual void Save (const string& input);
    virtual void Load (string* out) const;

    virtual bool editable () const { return true; }
private:
    Entity* m_entity;
};

class EntityScaleProperty : public Property {
public:
    EntityScaleProperty (const string& name, Entity* e)
        : Property(name), m_entity(e) { }
    virtual void Save (const string& input);
    virtual void Load (string* out) const;

    virtual bool editable () const { return true; }
private:
    Entity* m_entity;
};
