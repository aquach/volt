#pragma once

#include "Game/Core/Core.h"
#include "Volt/Assets/AssetManager.h"

namespace Volt {
    class Scene;
}

struct DoodadBrush {
	DoodadBrush ()
		: id(0) { }

	int id;
    Vector2 textureCoords[4];
    string texture;
    Vector2 size;
    string name;

    void Load (const Json::Value& node);
    void Save (Json::Value& node) const;
};

class DoodadManager {
public:
    DoodadManager () : m_scene(NULL) { }
    ~DoodadManager ();

    static void Register (DoodadManager* manager) { instance = manager; }
    static DoodadManager* Instance () { return instance; }

    void LoadDoodadBrushes (Volt::DataAssetRef brushes);

    DoodadBrush* GetDoodadBrush (int id);
    void GetDoodadBrushes (vector<DoodadBrush*>* brushes);

private:
    friend class GameScene;
    friend class EditorScene;
    Volt::Scene* m_scene;

    typedef map<int, DoodadBrush*> Brushes;
    Brushes m_brushes;

    static DoodadManager* instance;

    DISALLOW_COPY_AND_ASSIGN(DoodadManager);
};

#define G_DoodadManager DoodadManager::Instance()
