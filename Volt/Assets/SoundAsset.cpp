#include "SoundAsset.h"
#include <AL/al.h>
#include <ogg/ogg.h>
#include "Assets/AssetManager.h"
#include "Vorbis.h"
#include "DataSource.h"
#include "Assets/SoundManager.h"

namespace Volt {

const int BUFFER_SIZE = 4096 * 16;
const int CHUNK_SIZE = 4096;
const int OGG_LITTLE_ENDIAN = 0;
const int WORD_SIZE = 2;
const int SIGNED = 1;

static string GetVorbisErrorString (int code) {
    switch (code) {
        case OV_EFAULT: return "Fault";
        case OV_EIMPL: return "Impl";
        case OV_EINVAL: return "Invalid value.";
        case OV_ENOTVORBIS: return "Not vorbis file.";
        case OV_EBADHEADER: return "Bad header.";
        case OV_EVERSION: return "Version error.";
        case OV_ENOTAUDIO: return "Not audio.";
        case OV_EBADPACKET: return "Bad packet.";
        case OV_EBADLINK: return "Bad link.";
        case OV_ENOSEEK: return "No seek.";
        default: return "Unknown error.";
    }
}

/** Plays a sound that can't be layered, but can be repeated, stopped, and
 *  paused. Good for music which doesn't need to be layered. Streams the
 *  decompressed PCM data to minimize memory use. Can be used with arbitrary
 *  length files. */
class SingleSound {
public:
    SingleSound ()
        : m_info(NULL),
          m_comment(NULL),
          m_source(0),
          m_format(0) {
    }

    ~SingleSound () {
        delete[] m_file.data;
        alSourceStop(m_source);
        EmptyBuffers();
        alDeleteSources(1, &m_source);
        alDeleteBuffers(NUM_BUFFERS, m_buffers);
        ov_clear(&m_oggStream);
    }

    bool Load (const DataItem& item, float volume = 1.0) {
        ov_callbacks vorbisCallbacks;
        vorbisCallbacks.read_func = VorbisRead;
        vorbisCallbacks.close_func = VorbisClose;
        vorbisCallbacks.seek_func = VorbisSeek;
        vorbisCallbacks.tell_func = VorbisTell;

        m_file.data = new char[item.size];
        memcpy(m_file.data, item.data, item.size);
        m_file.size = item.size;
        m_file.readBytes = 0;

        int ret = ov_open_callbacks(&m_file, &m_oggStream, NULL, 0,
                                    vorbisCallbacks);
        if (ret != 0) {
            LOG(ERROR) << "Could not read OGG file from memory: "
                       << GetVorbisErrorString(ret);
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
        alSourcef (m_source, AL_GAIN,            volume       );

        return true;
    }

    void Play () {
        if (IsPlaying())
            return;

        ov_raw_seek(&m_oggStream, 0);

        for (int i = 0; i < NUM_BUFFERS; i++) {
            if (!Stream(m_buffers[i]))
                return;
        }

        alSourceQueueBuffers(m_source, 2, m_buffers);
        alSourcePlay(m_source);
    }
    bool PlayAt (Vector2 source, float volume) {
        // TODO: source ignored for now until you can bind listener to entity.
        return false;
    }

    bool IsPlaying () {
        ALenum state;
        alGetSourcei(m_source, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    void Stop () {
        alSourceStop(m_source);
        EmptyBuffers();
    }

    void Pause ();
    bool IsPaused ();

    void Update () {
        if (!IsPlaying())
            return;

        int numProcessed;
        bool active = true;

        alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &numProcessed);

        for (int i = 0; i < numProcessed; i++) {
            ALuint buffer;
            alSourceUnqueueBuffers(m_source, 1, &buffer);
            active = Stream(buffer);
            alSourceQueueBuffers(m_source, 1, &buffer);
        }

        if (!active) {
            Stop();
        }
    }

private:
    void EmptyBuffers () {
        int numQueued;
        alGetSourcei(m_source, AL_BUFFERS_QUEUED, &numQueued);
        for (int i = 0; i < numQueued; i++) {
            ALuint buffer;
            alSourceUnqueueBuffers(m_source, 1, &buffer);
        }
    }

    bool Stream (ALuint buffer) {
        char pcm[BUFFER_SIZE];
        int size = 0;
        int section;
        int result;

        while (size < BUFFER_SIZE) {
            result = ov_read(&m_oggStream, pcm + size, BUFFER_SIZE - size,
                             OGG_LITTLE_ENDIAN, WORD_SIZE, SIGNED, &section);
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

    OggFile m_file;
    OggVorbis_File m_oggStream;
    vorbis_info* m_info;
    vorbis_comment* m_comment;

    ALuint m_buffers[NUM_BUFFERS];
    ALuint m_source;
    ALenum m_format;
};

/** Can be played multiple times, stacked on top of each other.
 *  Buffers the entire decompressed PCM data in memory, so don't use this on
 *  anything longer than 10 seconds (1.6 MB)! */
class MultipleSound {
public:
    MultipleSound ()
        : m_info(NULL),
          m_comment(NULL),
          m_buffer(0),
          m_format(0),
          m_volume(0) {
    }

    ~MultipleSound () {
        delete[] m_file.data;
        for (list<ALuint>::iterator i = m_sources.begin(); i != m_sources.end();
             i++) {
            ALuint source = *i;
            alSourceStop(source);
            alDeleteSources(1, &source);
        }
        alDeleteBuffers(1, &m_buffer);
        ov_clear(&m_oggStream);
    }

    bool Load (const DataItem& item, float volume = 1.0) {
        m_volume = volume;

        ov_callbacks vorbisCallbacks;
        vorbisCallbacks.read_func = VorbisRead;
        vorbisCallbacks.close_func = VorbisClose;
        vorbisCallbacks.seek_func = VorbisSeek;
        vorbisCallbacks.tell_func = VorbisTell;

        m_file.data = new char[item.size];
        memcpy(m_file.data, item.data, item.size);
        m_file.size = item.size;
        m_file.readBytes = 0;

        int ret = ov_open_callbacks(&m_file, &m_oggStream, NULL, 0,
                                    vorbisCallbacks);
        if (ret != 0) {
            LOG(ERROR) << "Could not read OGG file from memory: "
                       << GetVorbisErrorString(ret);
            return false;
        }

        m_info = ov_info(&m_oggStream, -1);
        m_comment = ov_comment(&m_oggStream, -1);

        if (m_info->channels == 1)
            m_format = AL_FORMAT_MONO16;
        else
            m_format = AL_FORMAT_STEREO16;

        alGenBuffers(1, &m_buffer);
        LoadBuffer ();

        return true;
    }

    void Play () {
        ALuint source;
        alGenSources(1, &source);
        alSource3f(source, AL_POSITION,        0.0, 0.0, 0.0);
        alSource3f(source, AL_VELOCITY,        0.0, 0.0, 0.0);
        alSource3f(source, AL_DIRECTION,       0.0, 0.0, 0.0);
        alSourcef (source, AL_ROLLOFF_FACTOR,  0.0          );
        alSourcei (source, AL_SOURCE_RELATIVE, AL_TRUE      );
        alSourcef (source, AL_GAIN,            m_volume     );

        alSourceQueueBuffers(source, 1, &m_buffer);
        alSourcePlay(source);
        m_sources.push_back(source);
    }

    void Update () {
        for (list<ALuint>::iterator i = m_sources.begin(); i != m_sources.end();
             ) {
            ALuint source = *i;
            ALenum state;
            alGetSourcei(source, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING) {
                alSourceStop(source);
                alDeleteSources(1, &source);
                i = m_sources.erase(i);
            } else {
                i++;
            }
        }
    }

private:
    void LoadBuffer () {
        /* Load entire buffer. */
        vector<char> pcmBuffer;
        pcmBuffer.reserve(CHUNK_SIZE * 16);
        int readBytes = 0;
        int section;
        int result;

        while (true) {
            pcmBuffer.resize(pcmBuffer.size() + CHUNK_SIZE);
            result = ov_read(&m_oggStream, pcmBuffer.data() + readBytes,
                             CHUNK_SIZE, OGG_LITTLE_ENDIAN, WORD_SIZE, SIGNED,
                             &section);
            if (result > 0)
                readBytes += result;
            else if (result < 0)
                LOG(ERROR) << "OGG decoding error.";
            else
                break;
        }

        alBufferData(m_buffer, m_format, &pcmBuffer[0], readBytes,
                     m_info->rate);
    }

    OggFile m_file;
    OggVorbis_File m_oggStream;
    vorbis_info* m_info;
    vorbis_comment* m_comment;

    ALuint m_buffer;
    list<ALuint> m_sources;
    ALenum m_format;
    float m_volume;
};


SoundAsset::SoundAsset ()
    : m_single(NULL),
      m_multiple(NULL),
      m_volume(0.0) {
}

SoundAsset::~SoundAsset () {
    Unload();
}

bool SoundAsset::Load (const DataItem& item, SoundType type, float volume) {
    m_path = item.path;
    m_type = type;
    m_volume = volume;

    bool success = false;
    if (type == SOUND_SINGLE) {
        m_single = new SingleSound();
        success = m_single->Load(item, volume);
    } else {
        m_multiple = new MultipleSound();
        success = m_multiple->Load(item, volume);
    }

    if (success)
        G_SoundManager->RegisterSound(this);

    return success;
}

void SoundAsset::Reload () {
    Unload();
    DataItem item;
    m_manager->ReloadPath(this, &item);
    Load(item, m_type, m_volume);
}

void SoundAsset::Unload () {
    if (m_single)
        delete m_single;
    if (m_multiple)
        delete m_multiple;
    G_SoundManager->UnregisterSound(this);
}

void SoundAsset::Play () {
    if (m_type == SOUND_SINGLE)
        m_single->Play();
    else
        m_multiple->Play();
}

void SoundAsset::Stop () {
    if (m_type == SOUND_SINGLE)
        m_single->Stop();
}

bool SoundAsset::IsPlaying () {
    if (m_type == SOUND_SINGLE)
        return m_single->IsPlaying();
    return false;
}

void SoundAsset::Update () {
    if (m_type == SOUND_SINGLE)
        m_single->Update();
    else
        m_multiple->Update();
}

}
