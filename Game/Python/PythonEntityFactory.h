#pragma once

#include "Game/Core/Core.h"

class Entity;

namespace PythonEntityFactory {

Entity* CreateEntity (const string& path);

void GetEntityTypes (const string& searchPath, vector<string>* entityNames,
                     vector<string>* entityPaths);

}
