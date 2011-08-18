#include "EntityFactory.h"

EntityFactory::Types* EntityFactory::s_types = NULL;

Entity* EntityFactory::Create (const string& name) {
    EntityFactory::Types::iterator iter = s_types->find(name);
    if (iter == s_types->end())
        return NULL;
    return iter->second();
}

void EntityFactory::GetEntityTypes (vector<string>* types) {
    EntityFactory::Types* map = GetTypeMap();
    for (EntityFactory::Types::iterator i = map->begin();
         i != map->end(); i++) {
        types->push_back(i->first);
    }
}
