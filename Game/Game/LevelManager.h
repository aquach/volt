#pragma once

#include "Core/Core.h"
#include <set>
#include "Volt/Assets/AssetManager.h"
#include "Scenes/GameScene.h"

namespace Volt {
    class Entity;
}

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
    set<Volt::Entity*> m_entities;

    DISALLOW_COPY_AND_ASSIGN(LevelManager);
};
