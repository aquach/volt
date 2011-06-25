#include "GameScene.h"
#include "Entities/Game/Player.h"
#include "Entities/Game/Floor.h"
#include "Game/LevelManager.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Assets/AssetManager.h"

const float WORLD_TO_SCREEN_SCALE = 30;

GameScene::GameScene ()
    : m_player(NULL),
      m_levelManager(NULL) {

    Volt::G_PhysicsManager->SetGravity(Vector2(0, 30));
    Volt::G_PhysicsManager->SetDebugDraw(true);

    camera()->transform.scale.Set(
        WORLD_TO_SCREEN_SCALE,
        WORLD_TO_SCREEN_SCALE);

    m_player = new Player;
    Add(m_player);
    //Add(new Floor);

    m_levelManager = new LevelManager;
    m_levelManager->m_gameScene = this;
    m_levelManager->LoadLevel(Volt::G_AssetManager->GetData("world.json"));

    camera()->WatchEntity(m_player);
}

GameScene::~GameScene () {
    delete m_levelManager;
}

/*
void GameScene::Render () {
}
*/

void GameScene::Update () {
    Scene::Update();
}

void GameScene::OnBegin () {
}

void GameScene::OnEnd () {
}

void GameScene::OnKeyEvent (SDL_KeyboardEvent event) {
    m_player->OnKeyEvent(event);
    if (event.keysym.sym == SDLK_ESCAPE)
        m_game->Quit();
}

