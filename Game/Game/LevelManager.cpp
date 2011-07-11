#include "Game/Game/LevelManager.h"
#include <fstream>
#include "Volt/Assets/DataSource.h"
#include "Game/Game/Entity.h"
#include "Game/Entities/Game/Ladder.h"
#include "Game/Entities/Game/Sign.h"
#include "Game/Entities/Game/Triangle.h"

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

    // TODO: More generic way.
    const Json::Value& triangles = root["triangles"];
    for (uint i = 0; i < triangles.size(); i++) {
        const Json::Value& node = triangles[i];
        Triangle* tri = new Triangle;
        tri->Load(node);
        m_entities.insert(tri);
        m_scene->Add(tri);
    }

    const Json::Value& ladders = root["ladders"];
    for (uint i = 0; i < ladders.size(); i++) {
        const Json::Value& node = ladders[i];
        Ladder* ladder = new Ladder;
        ladder->Load(node);
        m_entities.insert(ladder);
        m_scene->Add(ladder);
    }

    const Json::Value& signs = root["signs"];
    for (uint i = 0; i < signs.size(); i++) {
        const Json::Value& node = signs[i];
        Sign* sign = new Sign;
        sign->Load(node);
        m_entities.insert(sign);
        m_scene->Add(sign);
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
    // TODO
    return false;
}
