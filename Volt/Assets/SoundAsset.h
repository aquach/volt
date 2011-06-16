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

    bool Play ();
    bool IsPlaying ();
    void Stop ();

    bool Update ();

    /* TODO: Pause capability, pan capabilities, stack multiple sounds. */
private:
    void EmptyBuffers ();
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
