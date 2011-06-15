#pragma once

#include "Core/Core.h"
#include <AL/al.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "Asset.h"

namespace Volt
{

class DataItem;

class SoundAsset : public Asset {
public:
    enum SoundType {
    };

    SoundAsset ();
    ~SoundAsset ();

    bool Load (const DataItem& item);

    void Reload ();
    void Unload ();

    void Play ();
    bool IsPlaying ();
    void Stop ();

    void Update ();
private:
    OggVorbis_File  m_oggStream;
    vorbis_info*    m_info;
    vorbis_comment* m_comment;

    ALuint m_buffers[2];
    ALuint m_source;
    ALenum m_format;
};

}
