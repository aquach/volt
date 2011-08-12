#include "Volt/Assets/SoundManager.h"
#include <AL/al.h>
#include <AL/alc.h>

namespace Volt {

SoundManager* SoundManager::instance = NULL;

SoundManager::SoundManager ()
    : m_device(NULL),
      m_context(NULL) {
    LOG(INFO) << "Initializing sound system...";

    m_device = alcOpenDevice(NULL);
    if (m_device == NULL && false) {
        LOG(FATAL) << "Failed to initialize sound device.";
        exit(1);
    }

    m_context = alcCreateContext(m_device, NULL);
    alcMakeContextCurrent(m_context);
}

SoundManager::~SoundManager () {
    LOG(INFO) << "Destroying sound system...";

    alcMakeContextCurrent(NULL);
    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}

void SoundManager::Update () {
    for (set<SoundAsset*>::iterator i = m_sounds.begin(); i != m_sounds.end();
         i++) {
        (*i)->Update();
    }

    CheckErrors();
}

void SoundManager::RegisterSound (SoundAsset* asset) {
    m_sounds.insert(asset);
}

void SoundManager::UnregisterSound (SoundAsset* asset) {
    m_sounds.erase(asset);
}

void SoundManager::CheckErrors () {
    int error;
    if ((error = alGetError()) != AL_NO_ERROR) {
        switch (error) {
            case AL_INVALID_NAME:
                LOG(ERROR) << "OpenAL invalid name.";
            break;
            case AL_INVALID_ENUM:
                LOG(ERROR) << "OpenAL invalid enum.";
            break;
            case AL_INVALID_VALUE:
                LOG(ERROR) << "OpenAL invalid value.";
            break;
            case AL_INVALID_OPERATION:
                LOG(ERROR) << "OpenAL invalid operation.";
            break;
            case AL_OUT_OF_MEMORY:
                LOG(ERROR) << "OpenAL out of memory.";
            break;
            default:
                LOG(ERROR) << "Unknown OpenAL error.";
            break;
        }
    }
}

}
