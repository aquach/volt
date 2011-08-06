#pragma once

#include "Game/Core/Core.h"
#include <map>

class SaveData {
public:

    SaveData () { }
    ~SaveData () { }

    static void Register (SaveData* data) { instance = data; }
    static SaveData* Instance () {
        CHECK_NOTNULL(instance);
        return instance;
    }

    void Load (string filename);
    void Save (string filename) const;

    // TODO: Let Python access data as dict? Let C++ access as Json value?

    bool GetFlag (string key);
    void SetFlag (string key, bool value);

    float GetFloat (string key);
    void SetFloat (string key, float value);

    int GetInt (string key);
    void SetInt (string key, int value);
    void Increment (string key, int amount);

private:

    static SaveData* instance;

    DISALLOW_COPY_AND_ASSIGN(SaveData);
};

#define G_SaveData SaveData::Instance()

}
