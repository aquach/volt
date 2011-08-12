#pragma once

#include "Game/Core/Core.h"
#include <map>

class SaveData {
public:

    SaveData (string filename) : m_filename(filename) { }
    ~SaveData () { }

    static void Register (SaveData* data) { instance = data; }
    static SaveData* Instance () {
        CHECK_NOTNULL(instance);
        return instance;
    }

    bool Load ();
    void Unload ();
    bool Save () const;

    bool GetFlag (string key);
    void SetFlag (string key, bool value);

    float GetFloat (string key);
    void SetFloat (string key, float value);

    int GetInt (string key);
    void SetInt (string key, int value);
    void Increment (string key, int amount);

private:
    string m_filename;
    Json::Value m_root;
    static SaveData* instance;

    DISALLOW_COPY_AND_ASSIGN(SaveData);
};

#define G_SaveData SaveData::Instance()
