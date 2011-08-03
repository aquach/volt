#pragma once

#include "Game/Core/Core.h"
#include <set>
#include "Volt/Assets/AssetManager.h"
#include "Game/Game/Entity.h"

namespace Volt {
    class Scene;
}

class LevelManager {
public:
    LevelManager (Volt::Scene* scene);
    ~LevelManager ();

    //void Update ();

    void LoadLevel (Volt::DataAssetRef asset);
    bool LoadLevelFromFilename (string filename);
    void UnloadLevel ();
    bool SaveLevel (string filename);

    string loadedFile () const { return m_loadedFilename; }
    string levelName () const { return m_levelName; }
    void SetLevelName (string name) { m_levelName = name; }

private:
    Volt::Scene* m_scene;

    bool m_levelLoaded;
    set<Entity*> m_entities;
    string m_loadedFilename;
    string m_levelName;

    DISALLOW_COPY_AND_ASSIGN(LevelManager);
};
