#pragma once

#include "Core/Core.h"
#include <AL/al.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "Asset.h"
#include "Vorbis.h"

namespace Volt {

const int NUM_BUFFERS = 2;

class DataItem;
class SoundManager;

/* A sound file. */
class SoundAsset : public Asset {
public:
    SoundAsset ();
    ~SoundAsset ();

    bool Load (const DataItem& item);

    void Reload ();
    void Unload ();

    /** Plays the sound file. Returns true if the music is playing or was
     *  succesfully started. */
    bool Play ();

    bool IsPlaying ();
    void Stop ();

    /** Streams and queues another buffers of data into the source queue when
     *  a buffer has been processed. When the music data has ended, stops the
     *  source.
     */
    void Update ();

    /* TODO: Pause capability, pan capabilities, stack multiple sounds. */
private:
    void EmptyBuffers ();

    /** Reads another chunk of the OGG, decompressing it into PCM data and
     *  loading it into buffer. Returns true if the OGG has data left. */
    bool Stream (ALuint buffer);

    OggFile m_file;
    OggVorbis_File m_oggStream;
    vorbis_info* m_info;
    vorbis_comment* m_comment;

    ALuint m_buffers[NUM_BUFFERS];
    ALuint m_source;
    ALenum m_format;
};

}
