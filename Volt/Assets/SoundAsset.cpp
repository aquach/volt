#include "SoundAsset.h"
#include <AL/al.h>
#include <ogg/ogg.h>
#include "Assets/AssetManager.h"
#include "Vorbis.h"
#include "DataSource.h"
#include "Assets/SoundManager.h"

namespace Volt {

const int BUFFER_SIZE = 4096 * 16;

SoundAsset::SoundAsset ()
    : m_info(NULL),
      m_comment(NULL),
      m_source(0),
      m_format(0) {
}

SoundAsset::~SoundAsset () {
    Unload();
}

bool SoundAsset::Load (const DataItem& item) {
    m_path = item.path;

    ov_callbacks vorbisCallbacks;
    vorbisCallbacks.read_func = VorbisRead;
	vorbisCallbacks.close_func = VorbisClose;
	vorbisCallbacks.seek_func = VorbisSeek;
	vorbisCallbacks.tell_func = VorbisTell;

    m_file.data = new char[item.size];
    memcpy(m_file.data, item.data, item.size);
    m_file.size = item.size;
    m_file.readBytes = 0;

	if (ov_open_callbacks(&m_file, &m_oggStream, NULL, 0,
                          vorbisCallbacks) != 0) {
		LOG(ERROR) << "Could not read OGG file from memory.";
        return false;
    }

    m_info = ov_info(&m_oggStream, -1);
    m_comment = ov_comment(&m_oggStream, -1);

    if (m_info->channels == 1)
        m_format = AL_FORMAT_MONO16;
    else
        m_format = AL_FORMAT_STEREO16;

    alGenBuffers(2, m_buffers);
    alGenSources(1, &m_source);

    alSource3f(m_source, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(m_source, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(m_source, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (m_source, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (m_source, AL_SOURCE_RELATIVE, AL_TRUE      );

    G_SoundManager->RegisterSound(this);

    return true;
}

void SoundAsset::Reload () {
    Unload();
    DataItem item;
    m_manager->ReloadPath(this, &item);
    Load(item);
}

void SoundAsset::Unload () {
    delete[] m_file.data;
    alSourceStop(m_source);
    // This doesn't seem necessary.
    //EmptyBuffers();
    alDeleteSources(1, &m_source);
    alDeleteBuffers(NUM_BUFFERS, m_buffers);
    ov_clear(&m_oggStream);
    G_SoundManager->UnregisterSound(this);
}

bool SoundAsset::Play () {
    if (IsPlaying())
        return true;

    for (int i = 0; i < NUM_BUFFERS; i++) {
        if (!Stream(m_buffers[i]))
            return false;
    }

    alSourceQueueBuffers(m_source, 2, m_buffers);
    alSourcePlay(m_source);

    return true;
}

void SoundAsset::Stop () {
    alSourceStop(m_source);
}

void SoundAsset::EmptyBuffers () {
    int numQueued;
    alGetSourcei(m_source, AL_BUFFERS_QUEUED, &numQueued);
    for (int i = 0; i < numQueued; i++) {
        ALuint buffer;
        alSourceUnqueueBuffers(m_source, 1, &buffer);
    }
}

bool SoundAsset::IsPlaying () {
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool SoundAsset::Update () {
    int numProcessed;
    bool active = true;

    alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &numProcessed);

    for (int i = 0; i < numProcessed; i++) {
        ALuint buffer;
        alSourceUnqueueBuffers(m_source, 1, &buffer);
        active = Stream(buffer);
        alSourceQueueBuffers(m_source, 1, &buffer);
    }

    return active;
}

bool SoundAsset::Stream (ALuint buffer) {
    char pcm[BUFFER_SIZE];
    int size = 0;
    int section;
    int result;

    while (size < BUFFER_SIZE) {
        result = ov_read(&m_oggStream, pcm + size, BUFFER_SIZE - size, 0, 2, 1,
                         &section);
        if (result > 0)
            size += result;
        else if (result < 0)
            LOG(ERROR) << "OGG decoding error.";
        else
            break;
    }
    if (size == 0)
        return false;

    alBufferData(buffer, m_format, pcm, size, m_info->rate);
    return true;
}

}
