#include "Editor/Editor/EditorScene.h"
#include "Game/Game/LevelManager.h"
#include "Editor/Editor/Editor.h"

const float WORLD_TO_SCREEN_SCALE = 30;
const char* EDITOR_TITLE = "Endmill Editor";

EditorScene::EditorScene ()
    : m_levelManager(NULL),
      m_editor(NULL),
      m_cameraMoveX(0),
      m_cameraMoveY(0) {

    camera()->transform.scale.Set(
        WORLD_TO_SCREEN_SCALE,
        WORLD_TO_SCREEN_SCALE);

    m_levelManager = new LevelManager;
    m_levelManager->m_scene = this;
    m_levelManager->LoadLevel(Volt::G_AssetManager->GetData("world.json"));
}

EditorScene::~EditorScene () {
}

void EditorScene::Update () {
    Scene::Update();

    camera()->transform.position += Vector2(
        m_cameraMoveX * 20,
        m_cameraMoveY * 20) * m_editor->dt();
}

void EditorScene::OnBegin () {
}

void EditorScene::OnEnd () {
}
