#pragma once

#include "Volt/Game/Entity.h"

class Property;

class Entity : public Volt::Entity {
public:
    virtual void GetProperties (vector<Property*>* properties);
private:
};
