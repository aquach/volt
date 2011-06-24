#include "LevelManager.h"
#include "Entities/Game/Area.h"
#include "Entities/Game/Light.h"
#include "Entities/Game/Triangle.h"

LevelManager::LevelManager ()
    : m_levelLoaded(false),
      m_gameScene(NULL) {
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
    for (int i = 0; i < triangles.size(); i++) {
        const Json::Value& triNode = triangles[i];
        Triangle* tri = new Triangle;
        tri->Load(triNode);
        m_triangles.insert(tri);
        m_gameScene->Add(tri);
    }
}

void LevelManager::UnloadLevel () {
    for (set<Triangle*>::iterator i = m_triangles.begin(); i != m_triangles.end(); i++)
        m_gameScene->Remove(*i);
    for (set<Light*>::iterator i = m_lights.begin(); i != m_lights.end(); i++)
        m_gameScene->Remove(*i);
    for (set<Area*>::iterator i = m_areas.begin(); i != m_areas.end(); i++)
        m_gameScene->Remove(*i);
}