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
    explicit LevelManager (Volt::Scene* scene);
    ~LevelManager ();

    //void Update ();

    void LoadLevel (Volt::DataAssetRef asset);
    bool LoadLevelFromFilename (const string& filename);
    void UnloadLevel ();
    bool SaveLevel (const string& filename);

    string loadedFile () const { return m_loadedFilename; }
    string levelName () const { return m_levelName; }
    void SetLevelName (const string& name) { m_levelName = name; }

private:
    Volt::Scene* m_scene;

    bool m_levelLoaded;
    set<Entity*> m_entities;
    string m_loadedFilename;
    string m_levelName;

    DISALLOW_COPY_AND_ASSIGN(LevelManager);
};
