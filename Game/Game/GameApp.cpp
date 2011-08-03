#include "Game/Game/GameApp.h"
#include "Game/Python/Python.h"

GameApp::GameApp (const string& name, int argc, char** argv,
                  const Volt::DataSource* source)
    : Volt::Game(name, source){
    Python::Initialize(argc, argv);
}

GameApp::~GameApp () {
}
