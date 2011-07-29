#include "Editor/Editor/EditorScene.h"
#include "Volt/Game/AppTime.h"
#include "Game/Editor/SelectionManager.h"
#include "Game/Entities/Game/Ladder.h"
#include "Game/Game/DoodadManager.h"
#include "Game/Game/LevelManager.h"
#include "Game/Game/LightManager.h"
#include "Editor/Editor/Editor.h"

const float WORLD_TO_SCREEN_SCALE = 30;
const char* EDITOR_TITLE = "Endmill Editor";

EditorScene::EditorScene ()
    : m_levelManager(NULL),
      m_doodadManager(NULL),
      m_editor(NULL),
      m_cameraMoveX(0),
      m_cameraMoveY(0) {

    m_isEditor = true;

    camera()->transform.scale.Set(
        WORLD_TO_SCREEN_SCALE,
        WORLD_TO_SCREEN_SCALE);

    m_levelManager = new LevelManager;
    m_levelManager->m_scene = this;

    m_doodadManager = new DoodadManager;
    m_doodadManager->m_scene = this;
    m_doodadManager->LoadDoodadBrushes(
        G_AssetManager->GetData("doodads.json"));
    DoodadManager::Register(m_doodadManager);

    m_lightManager = new LightManager;
    m_lightManager->m_scene = this;
    LightManager::Register(m_lightManager);
}

EditorScene::~EditorScene () {
    delete m_doodadManager;
    delete m_levelManager;
    delete m_lightManager;
}

void EditorScene::Update () {
    Scene::Update();

    camera()->transform.position += Vector2(
        m_cameraMoveX * 20,
        m_cameraMoveY * 20) * Volt::G_Time->dt();
}

void EditorScene::OnPostRender () {
    G_SelectionManager->Render();
}

void EditorScene::OnBegin () {
}

void EditorScene::OnEnd () {
}
