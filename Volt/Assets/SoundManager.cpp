#include "SoundManager.h"
#include <AL/al.h>
#include <AL/alc.h>

namespace Volt {

SoundManager* SoundManager::instance = NULL;

SoundManager::SoundManager ()
    : m_device(NULL),
      m_context(NULL) {
    LOG(INFO) << "Initializing sound system...";

    m_device = alcOpenDevice(NULL);
    if (m_device == NULL) {
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

}
