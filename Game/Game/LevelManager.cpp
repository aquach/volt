#include "Game/Game/LevelManager.h"
#include <fstream>
#include "Volt/Assets/DataSource.h"
#include "Game/Game/Entity.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Editor/EditorEntities.h"

LevelManager::LevelManager ()
    : m_levelLoaded(false),
      m_scene(NULL) {
}

LevelManager::~LevelManager () {
    UnloadLevel();
}

/*
void LevelManager::Update () {
}
*/

void LevelManager::LoadLevel (Volt::DataAssetRef asset) {
    if (m_levelLoaded)
        UnloadLevel();

    const Json::Value& root = asset->data();
    CHECK(root.isMember("entities"));

    m_levelName = root.get("name", "").asString();
    for (uint i = 0; i < root["entities"].size(); i++) {
        const Json::Value& node = root["entities"][i];
        CHECK(node.isMember("type"));
        Entity* e = EntityFactory::Create(node["type"].asString());
        e->Load(node);
        m_entities.insert(e);
        m_scene->Add(e);
    }
}

bool LevelManager::LoadLevelFromFilename (string filename) {
    ifstream file(filename.c_str(), ifstream::binary);
    if (!file.is_open())
        return false;

    file.seekg(0, ios::end);
    int size = file.tellg();
    file.seekg(0, ios::beg);

    char* data = new char[size];
    file.read(data, size);

    Volt::DataItem item;
    item.size = size;
    item.data = data;

    Volt::DataAsset asset;
    bool success = asset.Load(item);
    if (!success)
        return false;

    LoadLevel(Volt::DataAssetRef(&asset));

    m_loadedFilename = filename;

    return true;
}

void LevelManager::UnloadLevel () {
    FOR_ (set<Entity*>::iterator, i, m_entities)
        m_scene->Remove(*i);
    m_loadedFilename = "";
}

bool LevelManager::SaveLevel (string filename) {
    ofstream file(filename.c_str(), ifstream::binary);
    if (!file.is_open())
        return false;

    Json::Value root;
    Json::Value entitiesNode;
    vector<Volt::Entity*> entities;
    m_scene->GetEntities(&entities);
    for (uint i = 0; i < entities.size(); i++) {
        if (Entity* e = dynamic_cast<Entity*>(entities[i])) {
            Json::Value value;
            e->Save(value);
            if (!value.empty())
                entitiesNode.append(value);
        }
    }

    root["entities"] = entitiesNode;
    root["name"] = m_levelName;

    file << root;
    file.close();

    return true;
}
