#include "Game/Game/LevelManager.h"
#include <fstream>
#include <Python.h>
#include "Volt/Assets/DataSource.h"
#include "Volt/Python/Python.h"
#include "Game/Game/Entity.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Editor/EditorEntities.h"

LevelManager* LevelManager::instance = NULL;

LevelManager::LevelManager (Volt::Scene* scene)
    : m_scene(scene),
      m_levelUnloading(false),
      m_pythonEnabled(false){
}

LevelManager::~LevelManager () {
    UnloadLevel();
}

void LevelManager::Update () {
    if (m_switchToAssetName.size() > 0) {
        LoadLevelFromAssetName(m_switchToAssetName);
        m_switchToAssetName = "";
    }
}

Entity* CreatePythonEntity(const string& path) {
    long result = 0;

    PyObject* moduleName = Py_BuildValue("s", "entity_factory");
    PyObject* module = PyImport_Import(moduleName);
    if (module) {
        PyObject* moduleDict = PyModule_GetDict(module);
        PyObject* func = PyDict_GetItemString(moduleDict, "createEntity");
        if (func != NULL && PyCallable_Check(func)) {
            PyObject* args = Py_BuildValue("(s)", path.c_str());
            PyObject* returnValue = PyObject_CallObject(func, args);
            if (returnValue) {
                result = PyLong_AsLong(returnValue);
                Py_XDECREF(returnValue);
            }
            Py_XDECREF(args);
        }
        Py_XDECREF(module);
    }
    Py_XDECREF(moduleName);

    return (Entity*)result;
}

void LevelManager::LoadLevel (Volt::DataAssetRef asset) {
    if (m_loadedFilename.size() > 0)
        UnloadLevel();

    const Json::Value& root = asset->data();
    CHECK(root.isMember("entities"));

    m_levelName = root.get("name", "").asString();
    for (uint i = 0; i < root["entities"].size(); i++) {
        const Json::Value& node = root["entities"][i];
        CHECK(node.isMember("type"));
        Entity* e = EntityFactory::Create(node["type"].asString());
        e->Load(node);
        m_entities.insert(e);
        m_scene->Add(e, e->layer());
    }

    m_startScript = root.get("startScript", "").asString();
    if (m_startScript.size() > 0 && m_pythonEnabled) {
        LOG(INFO) << "Executing start script " << m_startScript;
        Volt::Python::RunGameScriptFile(m_startScript);
    }

    m_loadedFilename = asset->path();
}

void LevelManager::LoadLevelFromAssetName (const string& assetPath) {
    LoadLevel(G_AssetManager->GetData(assetPath));
}

bool LevelManager::LoadLevelFromFilename (const string& filename) {
    ifstream file(filename.c_str(), ifstream::binary);
    if (!file.is_open())
        return false;

    file.seekg(0, ios::end);
    int size = file.tellg();
    file.seekg(0, ios::beg);

    char* data = new char[size];
    file.read(data, size);

    // Destructor of DataItem frees data.
    Volt::DataItem item;
    item.size = size;
    item.data = data;

    Volt::DataAsset asset;
    bool success = asset.Load(item);
    if (!success)
        return false;

    LoadLevel(Volt::DataAssetRef(&asset));

    m_loadedFilename = filename;

    return true;
}

void LevelManager::UnloadLevel () {
    LOG(INFO) << "Unloading level...";
    m_levelUnloading = true;
    if (m_pythonEnabled)
        Volt::Python::WaitForScripts();
    FOR_ (set<Entity*>::iterator, i, m_entities)
        m_scene->Remove(*i);
    m_loadedFilename = "";
    m_levelUnloading = false;
}

bool LevelManager::SaveLevel (const string& filename) {
    ofstream file(filename.c_str(), ifstream::binary);
    if (!file.is_open())
        return false;

    Json::Value root;
    Json::Value entitiesNode;
    vector<Volt::Entity*> entities;
    m_scene->GetEntities(&entities);
    for (uint i = 0; i < entities.size(); i++) {
        if (Entity* e = dynamic_cast<Entity*>(entities[i])) {
            Json::Value value;
            e->Save(value);
            if (!value.empty())
                entitiesNode.append(value);
        }
    }

    root["entities"] = entitiesNode;
    root["name"] = m_levelName;
    root["startScript"] = m_startScript;

    file << root;
    file.close();

    return true;
}
