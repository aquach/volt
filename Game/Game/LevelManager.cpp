#include "Game/Game/LevelManager.h"
#include "Game/Entities/Game/Ladder.h"
#include "Game/Entities/Game/Triangle.h"
#include "Volt/Game/Entity.h"

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
}

void LevelManager::UnloadLevel () {
    FOR_ (set<Volt::Entity*>::iterator, i, m_entities)
        m_scene->Remove(*i);
}
