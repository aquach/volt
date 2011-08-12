#include "Game/Game/GameApp.h"
#include "Volt/Python/Python.h"
#include "Game/Game/SaveData.h"
#include "Game/Python/Game_wrap.cxx"

GameApp::GameApp (const string& name, int argc, char** argv,
                  const Volt::DataSource* source)
    : Volt::Game(name, source){

    m_saveData = new SaveData("player.sav");
    SaveData::Register(m_saveData);

    Volt::Python::Initialize(argc, argv);

    // Import Python bindings into module directly.
    SWIG_init();
    PyRun_SimpleString(
        G_AssetManager->GetScript("Scripts/bootstrap.py")->script().c_str());

}

GameApp::~GameApp () {
    delete m_saveData;
}
