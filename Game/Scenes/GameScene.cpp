#include "Game/Scenes/GameScene.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Volt/Graphics/SDLWindow.h"
#include "Volt/Graphics/Viewport.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Entities/Game/Light.h"
#include "Game/Entities/Game/Player.h"
#include "Game/Entities/Game/Umbrella.h"
#include "Game/Filters/EdgeFilter.h"
#include "Game/Game/ConversationManager.h"
#include "Game/Game/DoodadManager.h"
#include "Game/Game/Entity.h"
#include "Game/Game/LevelManager.h"
#include "Game/Game/LightManager.h"
#include "Game/Game/GamePerfHook.h"
#include "Game/Python/Python.h"
#include "Game/Python/ScriptConsole.h"

const float WORLD_TO_SCREEN_SCALE = 30;

GameScene::GameScene ()
    : m_player(NULL),
      m_levelManager(NULL),
      m_conversationManager(NULL),
      m_doodadManager(NULL),
      m_scriptConsole(NULL) {

    m_scriptConsole = new ScriptConsole;
    Add(m_scriptConsole, -50);

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

    m_doodadManager = new DoodadManager(this);
    m_doodadManager->LoadDoodadBrushes(
        G_AssetManager->GetData("doodads.json"));
    DoodadManager::Register(m_doodadManager);

    m_levelManager = new LevelManager(this);
    m_levelManager->LoadLevel(G_AssetManager->GetData("Levels/world.json"));
    LevelManager::Register(m_levelManager);

    m_conversationManager = new ConversationManager(this);

    m_lightManager = new LightManager(this);
    LightManager::Register(m_lightManager);

    SetHook(new GamePerfHook);

    camera()->WatchEntity(m_player);

    //EdgeFilter* f = new EdgeFilter("EdgeFilter", 40, -5);
    //AddFilter(f);
}

GameScene::~GameScene () {
    delete m_conversationManager;
    delete m_levelManager;
    delete m_doodadManager;
    delete m_lightManager;
}

void GameScene::Update () {
    Scene::Update();
    m_conversationManager->Update();
    m_levelManager->Update();
}

void GameScene::OnBegin () {
    Python::RunGameScriptFile("test.py");
}

void GameScene::OnEnd () {
}

void GameScene::SetPlayerInputLock (bool lock) {
    m_player->SetInputLock(lock);
}

void GameScene::OnKeyEvent (SDL_KeyboardEvent event) {
    bool handled = m_scriptConsole->OnKeyEvent(event);
    if (handled)
        return;

    m_conversationManager->OnKeyEvent(event);
    m_player->OnKeyEvent(event);
    if (event.type == SDL_KEYDOWN) {
        switch (event.keysym.sym) {
            case SDLK_ESCAPE:
                m_game->Quit();
            break;
            case SDLK_F1:
                Volt::G_PhysicsManager->ToggleDebugDraw();
            break;
            case SDLK_F2:
                // Reload all shaders.
                G_AssetManager->ReloadShaders();
            break;
            case SDLK_F9:
                dynamic_cast<Volt::SDLWindow*>(G_Viewport)->Screenshot("screenshot.bmp");
            break;
            default:
            break;
        }
    }
}

void GameScene::ShowMessageBox (MessageBox* box) {
    m_conversationManager->ShowMessageBox(box);
}
