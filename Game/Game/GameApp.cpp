#include "Game/Game/GameApp.h"
#include "Volt/Python/Python.h"

GameApp::GameApp (const string& name, int argc, char** argv,
                  const Volt::DataSource* source)
    : Volt::Game(name, source){
    Volt::Python::Initialize(argc, argv);
}

GameApp::~GameApp () {
}
