#pragma once

#include "Core/Core.h"
#include <set>
#include "Volt/Assets/AssetManager.h"
#include "Scenes/GameScene.h"

class Area;
class Light;
class Triangle;

class LevelManager {
public:
    LevelManager ();
    ~LevelManager ();

    //void Update ();

    void LoadLevel (Volt::DataAssetRef asset);
    void UnloadLevel ();

private:
    friend class GameScene;
    GameScene* m_gameScene;

    bool m_levelLoaded;
    set<Triangle*> m_triangles;
    set<Light*> m_lights;
    set<Area*> m_areas;

    DISALLOW_COPY_AND_ASSIGN(LevelManager);
};