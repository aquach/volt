#include "Game/Game/GameApp.h"
#include "Volt/Python/Python.h"
#include "Game/Python/Game_wrap.cxx"

GameApp::GameApp (const string& name, int argc, char** argv,
                  const Volt::DataSource* source)
    : Volt::Game(name, source){
    Volt::Python::Initialize(argc, argv);
    SWIG_init();
    PyRun_SimpleString(
        G_AssetManager->GetScript("Scripts/bootstrap.py")->script().c_str());
}

GameApp::~GameApp () {
}
