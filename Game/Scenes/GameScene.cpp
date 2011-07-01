#include "GameScene.h"
#include "Entities/Game/Player.h"
#include "Entities/Game/Projectile.h"
#include "Entities/Game/Creatures/Blobby.h"
#include "Entities/Game/Umbrella.h"
#include "Entities/Graphics/ParticleSystem.h"
#include "Entities/GUI/MessageBox.h"
#include "Game/ConversationManager.h"
#include "Game/LevelManager.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/PhysicsManager.h"

const float WORLD_TO_SCREEN_SCALE = 30;

GameScene::GameScene ()
    : m_player(NULL),
      m_levelManager(NULL),
      m_conversationManager(NULL) {

    Volt::G_PhysicsManager->SetGravity(Vector2(0, 30));
    Volt::G_PhysicsManager->SetDebugDraw(true);

    camera()->transform.scale.Set(
        WORLD_TO_SCREEN_SCALE,
        WORLD_TO_SCREEN_SCALE);

    m_player = new Player;
    Add(m_player);
    Umbrella* u = new Umbrella;
    Add(u, -1);
    m_player->EquipWeapon(u);

    Add(new Projectile);
    ParticleSystemDef def;
    Add(new ParticleSystem(Vector2(3, -3), def));

    Blobby* b = new Blobby;
    b->SetPosition(Vector2(3, -3));
    Add(b);

    m_levelManager = new LevelManager;
    m_levelManager->m_gameScene = this;
    m_levelManager->LoadLevel(Volt::G_AssetManager->GetData("world.json"));

    m_conversationManager = new ConversationManager;
    m_conversationManager->m_gameScene = this;

    MessageBoxDef boxDef;
    boxDef.text = "I'M A TEXT BOX! HOW ARE YOU?";
    MessageBox* box = new MessageBox(boxDef);
    ShowMessageBox(box);

    camera()->WatchEntity(m_player);
}

GameScene::~GameScene () {
    delete m_levelManager;
    delete m_conversationManager;
}

/*
void GameScene::Render () {
}
*/

void GameScene::Update () {
    Scene::Update();
    m_conversationManager->Update();
}

void GameScene::OnBegin () {
}

void GameScene::OnEnd () {
}

void GameScene::SetPlayerInputLock (bool lock) {
    m_player->SetInputLock(lock);
}

void GameScene::OnKeyEvent (SDL_KeyboardEvent event) {
    m_conversationManager->OnKeyEvent(event);
    m_player->OnKeyEvent(event);
    if (event.keysym.sym == SDLK_ESCAPE)
        m_game->Quit();
}

void GameScene::ShowMessageBox (MessageBox* box) {
    m_conversationManager->ShowMessageBox(box);
}
