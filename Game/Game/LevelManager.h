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

    void Update ();
    static void Register (LevelManager* manager) { instance = manager; }
    static LevelManager* Instance () { return instance; }

    void LoadLevel (Volt::DataAssetRef asset);
    void LoadLevelFromAssetName (const string& assetName);
    bool LoadLevelFromFilename (const string& filename);
    void UnloadLevel ();
    bool SaveLevel (const string& filename);

    void RequestLevelChange (const string& assetName) {
        m_switchToAssetName = assetName;
    }

    const string& loadedFile () const { return m_loadedFilename; }
    const string& levelName () const { return m_levelName; }
    void SetLevelName (const string& name) { m_levelName = name; }

    bool IsUnloading () const { return m_levelUnloading; }

private:
    Volt::Scene* m_scene;

    bool m_levelUnloading;
    set<Entity*> m_entities;
    string m_loadedFilename;
    string m_levelName;
    string m_startScript;
    string m_switchToAssetName; // Level to load on next update.

    static LevelManager* instance;

    DISALLOW_COPY_AND_ASSIGN(LevelManager);
};

#define G_LevelManager LevelManager::Instance()
