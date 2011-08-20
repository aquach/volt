#include "Game/Scenes/GameScene.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/SDLWindow.h"
#include "Volt/Graphics/SpriteAnimation.h"
#include "Volt/Graphics/Viewport.h"
#include "Game/Editor/EditorEntities.h"
#include "Game/Entities/Game/Humanoid.h"
#include "Game/Game/ConversationManager.h"
#include "Game/Game/DoodadManager.h"
#include "Game/Game/Entity.h"
#include "Game/Game/LevelManager.h"
#include "Game/Game/LightManager.h"
#include "Game/Game/GamePerfHook.h"
#include "Game/Python/ScriptConsole.h"

const float WORLD_TO_SCREEN_SCALE = 30;

GameScene::GameInputListener::~GameInputListener () {
    if (m_gameScene != NULL)
        m_gameScene->RemoveInputListener(this);
}

GameScene::GameScene ()
    : m_levelManager(NULL),
      m_conversationManager(NULL),
      m_doodadManager(NULL),
      m_scriptConsole(NULL),
      m_updateFreeze(false) {

    m_scriptConsole = new ScriptConsole;
    Add(m_scriptConsole, -50);

    Volt::G_PhysicsManager->SetGravity(Vector2(0, 30));
    Volt::G_PhysicsManager->SetDebugDraw(true);

    camera()->transform.scale.Set(
        WORLD_TO_SCREEN_SCALE,
        WORLD_TO_SCREEN_SCALE);

    Humanoid* player = new Humanoid;
    player->AddTag("Player");
    Volt::DataAssetRef animation = G_AssetManager->GetData(
        "Sprites/Player/player.json");
    player->SetAnimation(new Volt::SpriteAnimation(player, animation));
    Add(player, -2);
    player->CreateInputListener(this);
    camera()->WatchEntity(player);
    //Umbrella* u = new Umbrella;
    //Add(u, -2);
    //m_player->EquipWeapon(u);

    m_doodadManager = new DoodadManager(this);
    m_doodadManager->LoadDoodadBrushes(
        G_AssetManager->GetData("doodads.json"));
    DoodadManager::Register(m_doodadManager);

    m_levelManager = new LevelManager(this);
    m_levelManager->SetPythonInitScriptsEnabled(true);
    LevelManager::Register(m_levelManager);

    m_conversationManager = new ConversationManager(this);

    m_lightManager = new LightManager(this);
    LightManager::Register(m_lightManager);

    SetHook(new GamePerfHook);

    //EdgeFilter* f = new EdgeFilter("EdgeFilter", 40, -5);
    //AddFilter(f);
}

GameScene::~GameScene () {
    FOR_(InputListeners::iterator, i, m_inputListeners) {
        FOR_(set<GameInputListener*>::iterator, iter, i->second) {
            RemoveInputListener(*iter);
        }
    }

    delete m_conversationManager;
    delete m_levelManager;
    delete m_doodadManager;
    delete m_lightManager;
}

void GameScene::Update () {
    if (!m_updateFreeze)
        Scene::Update();
    m_conversationManager->Update();
    m_levelManager->Update();
}

void GameScene::OnBegin () {
    m_levelManager->LoadLevel(G_AssetManager->GetData("Levels/world.json"));
    ResolveEntityChanges();
}

void GameScene::OnEnd () {
}

void GameScene::OnKeyEvent (SDL_KeyboardEvent event) {
    bool handled = false;
    FOR_(InputListeners::iterator, i, m_inputListeners) {
        FOR_(set<GameInputListener*>::iterator, iter, i->second) {
            if ((*iter)->OnKeyEvent(event)) {
                handled = true;
                break;
            }
        }
        if (handled)
            break;
    }

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

void GameScene::ShowDialogBox (DialogBox* box) {
    m_conversationManager->ShowDialogBox(box);
}
