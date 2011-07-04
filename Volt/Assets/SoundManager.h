#pragma once

#include "Volt/Core/Core.h"
#include <AL/alc.h>
#include <set>
#include "Volt/Assets/AssetManager.h"

namespace Volt {

class SoundManager {
public:
    SoundManager ();
    ~SoundManager ();

    static void Register (SoundManager* manager) {
        instance = manager;
    }
    static SoundManager* Instance () { return instance; }

    void Update ();

    void RegisterSound (SoundAsset* asset);
    void UnregisterSound (SoundAsset* asset);

private:
    static SoundManager* instance;

    void CheckErrors ();

    set<SoundAsset*> m_sounds;
    ALCdevice* m_device;
    ALCcontext* m_context;

    DISALLOW_COPY_AND_ASSIGN(SoundManager);
};

#define G_SoundManager SoundManager::Instance()

}
