#include "GameScene.h"
#include "Entities/Game/Player.h"
#include "Entities/Game/Floor.h"
#include "Volt/Game/PhysicsManager.h"

const float WORLD_TO_SCREEN_SCALE = 30;

GameScene::GameScene ()
    : m_player(NULL) {

    Volt::G_PhysicsManager->SetGravity(Vector2(0, 10));

    camera()->transform.scale.Set(
        WORLD_TO_SCREEN_SCALE,
        WORLD_TO_SCREEN_SCALE);

    m_player = new Player;
    Add(m_player);
    Add(new Floor);

    camera()->WatchEntity(m_player);
}

GameScene::~GameScene () {
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

