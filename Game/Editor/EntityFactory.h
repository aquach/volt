#pragma once

#include "Game/Core/Core.h"

class Entity;

template<typename T> Entity* createEntity () { return new T; }

class EntityFactory {
public:
    typedef map<string, Entity*(*)()> Types;

    EntityFactory () { }
    ~EntityFactory () { }

    static Entity* Create (string name);
    static void GetEntityTypes (vector<string>* types);

protected:
    static Types* GetTypeMap () {
        if (s_types == NULL)
            s_types = new Types;
        return s_types;
    }
private:
    static Types* s_types;

    DISALLOW_COPY_AND_ASSIGN(EntityFactory);
};

template<typename T> class EntityRegister : public EntityFactory {
public:
    explicit EntityRegister (const string& name) {
        GetTypeMap()->insert(make_pair(name, &createEntity<T>));
        LOG(INFO) << name;
    }
};

#define DECLARE_ENTITY_(TYPE) \
    static EntityRegister<TYPE> TYPE##Register;
    
#define REGISTER_ENTITY_(TYPE) \
    EntityRegister<TYPE> TYPE::TYPE##Register(#TYPE);
