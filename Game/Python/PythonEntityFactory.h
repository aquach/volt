#pragma once

#include "Game/Core/Core.h"

class Entity;

namespace PythonEntityFactory {

Entity* CreateEntity (int id);

void GetEntityTypes (const string& searchPath, vector<string>* entityNames,
                     vector<int>* entityIds);

}
