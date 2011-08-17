#include "Game/Game/GameApp.h"
#include "Volt/Python/Python.h"
#include "Game/Game/SaveData.h"
#include "Game/Python/Python.h"

GameApp::GameApp (const string& name, int argc, char** argv,
                  const Volt::DataSource* source)
    : Volt::Game(name, source){

    m_saveData = new SaveData("player.sav");
    SaveData::Register(m_saveData);

    Volt::Python::Initialize(argc, argv);
    Python::Initialize();

}

GameApp::~GameApp () {
    delete m_saveData;
}
