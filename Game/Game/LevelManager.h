#pragma once

#include "Game/Core/Core.h"
#include <set>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/Scene.h"
#include "Game/Game/Entity.h"

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
    string levelName () const { return m_levelName; }
    void SetLevelName (string name) { m_levelName = name; }

private:
    friend class GameScene;
    friend class EditorScene;
    Volt::Scene* m_scene;

    bool m_levelLoaded;
    set<Entity*> m_entities;
    string m_loadedFilename;
    string m_levelName;

    DISALLOW_COPY_AND_ASSIGN(LevelManager);
};
