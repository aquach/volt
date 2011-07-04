#pragma once

#include "Game/Core/Core.h"
#include <set>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/Scene.h"

namespace Volt {
    class Entity;
}

class LevelManager {
public:
    LevelManager ();
    ~LevelManager ();

    //void Update ();

    void LoadLevel (Volt::DataAssetRef asset);
    bool LoadLevelFromFilename (string filename);
    void UnloadLevel ();
    bool SaveLevel (string filename);

    string loadedFile () const { return m_loadedFilename; }

private:
    friend class GameScene;
    friend class EditorScene;
    Volt::Scene* m_scene;

    bool m_levelLoaded;
    set<Volt::Entity*> m_entities;
    string m_loadedFilename;

    DISALLOW_COPY_AND_ASSIGN(LevelManager);
};
