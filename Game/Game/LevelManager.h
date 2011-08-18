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

    // Used to avoid switching levels halfway through a frame loop.
    void RequestLevelChange (const string& assetName) {
        m_switchToAssetName = assetName;
    }

    const string& loadedFile () const { return m_loadedFilename; }
    const string& levelName () const { return m_levelName; }
    void SetLevelName (const string& name) { m_levelName = name; }

    // Used to signal to scripts that the level is unloading.
    bool IsUnloading () const { return m_levelUnloading; }

    /* Activate/Deactivate Python functionality. Editor doesn't want to run
     * Python scripts. */
    void SetPythonInitScriptsEnabled (bool b) {
        m_pythonInitScriptsEnabled = b;
    }
    bool pythonInitScriptsEnabled () const {
        return m_pythonInitScriptsEnabled;
    }

    const string& startScript () const { return m_startScript; }
    void SetStartScript (const string& script) { m_startScript = script; }

private:
    Volt::Scene* m_scene;

    bool m_pythonInitScriptsEnabled;
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
