#pragma once

#include "Game/Core/Core.h"

class Entity;

class Property {
public:
    Property (string name)
        : m_name(name) { }
    virtual void Save (string input) = 0;
    virtual void Load (string* out) const = 0;

    virtual bool editable () const { return true; }

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

class EntityLayerProperty : public Property {
public:
    EntityLayerProperty (string name, Entity* e)
        : Property(name), m_entity(e) { }
    virtual void Save (string input);
    virtual void Load (string* out) const;

    virtual bool editable () const { return false; }
private:
    Entity* m_entity;
};

class EntityPositionProperty : public Property {
public:
    EntityPositionProperty (string name, Entity* e)
        : Property(name), m_entity(e) { }
    virtual void Save (string input);
    virtual void Load (string* out) const;

    virtual bool editable () const { return true; }
private:
    Entity* m_entity;
};

class EntityRotationProperty : public Property {
public:
    EntityRotationProperty (string name, Entity* e)
        : Property(name), m_entity(e) { }
    virtual void Save (string input);
    virtual void Load (string* out) const;

    virtual bool editable () const { return true; }
private:
    Entity* m_entity;
};

class EntityScaleProperty : public Property {
public:
    EntityScaleProperty (string name, Entity* e)
        : Property(name), m_entity(e) { }
    virtual void Save (string input);
    virtual void Load (string* out) const;

    virtual bool editable () const { return true; }
private:
    Entity* m_entity;
};
