#include "Editor/Editor/EditorScene.h"
#include "Game/Game/LevelManager.h"

const float WORLD_TO_SCREEN_SCALE = 30;

EditorScene::EditorScene ()
    : m_levelManager(NULL) {

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
}

void EditorScene::OnBegin () {
}

void EditorScene::OnEnd () {
}
