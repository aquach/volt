#pragma once

#include "Core/Core.h"
#include <AL/alc.h>
#include "AssetManager.h"

namespace Volt {

class SoundManager {
public:
    SoundManager ();
    ~SoundManager ();

    static void Register (SoundManager* manager) {
        instance = manager;
    }
    static SoundManager* Instance () { return instance; }

    void Play (SoundAssetRef sound);

private:
    static SoundManager* instance;

    ALCdevice* m_device;
    ALCcontext* m_context;

    DISALLOW_COPY_AND_ASSIGN(SoundManager);
};

#define G_SoundManager SoundManager::Instance()

}
