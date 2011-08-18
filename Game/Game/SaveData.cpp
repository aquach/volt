#include "Game/Game/SaveData.h"
#include <fstream>
#include "Volt/Assets/DataAsset.h"
#include "Volt/Assets/DataSource.h"

SaveData* SaveData::instance = NULL;

bool SaveData::Load () {
    ifstream file(m_filename.c_str(), ifstream::binary);
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

    m_root = asset.data();
    return true;
}

void SaveData::Unload () {
    m_root.clear();
}

bool SaveData::Save () const {
    ofstream file(m_filename.c_str(), ifstream::binary);
    if (!file.is_open())
        return false;

    file << m_root;
    file.close();
    return true;
}

bool SaveData::GetFlag (string key) {
    return m_root.get(key, false).asBool();
}

void SaveData::SetFlag (string key, bool value) {
    m_root[key] = value;
}

float SaveData::GetFloat (string key) {
    return m_root.get(key, 0.0f).asDouble();
}

void SaveData::SetFloat (string key, float value) {
    m_root[key] = value;
}

int SaveData::GetInt (string key) {
    return m_root.get(key, 0).asInt();
}

void SaveData::SetInt (string key, int value) {
    m_root[key] = value;
}

void SaveData::Increment (string key, int amount) {
    m_root[key] = m_root.get(key, 0).asInt() + 1;
}
