#include "EntityFactory.h"
#include "Game/Game/Entity.h"

EntityFactory::Types* EntityFactory::s_types = NULL;

Entity* EntityFactory::Create (string name) {
    EntityFactory::Types::iterator iter = s_types->find(name);
    CHECK(iter != s_types->end()) << "Invalid entity type " << name;
    return iter->second();
}

void EntityFactory::GetEntityTypes (vector<string>* types) {
    EntityFactory::Types* map = GetTypeMap();
    for (EntityFactory::Types::iterator i = map->begin();
         i != map->end(); i++) {
        types->push_back(i->first);
    }
}
