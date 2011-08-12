#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/Game.h"

namespace Volt {
    class DataSource;
}

class SaveData;

class GameApp : public Volt::Game {
public:
    GameApp (const string& name, int argc, char** argv,
             const Volt::DataSource* source);
    virtual ~GameApp ();

private:
    SaveData* m_saveData;
};
