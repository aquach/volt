#pragma once

#include "Volt/Game/Entity.h"
#include "Game/Editor/Property.h"

class Entity : public Volt::Entity {
public:
    virtual void GetProperties (vector<Property*>* properties);
private:
};
