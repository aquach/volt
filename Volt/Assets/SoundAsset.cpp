#include "SoundAsset.h"
#include <AL/al.h>
#include <ogg/ogg.h>
#include "Vorbis.h"
#include "DataSource.h"

namespace Volt {

const int BUFFER_SIZE = 4096 * 4;

SoundAsset::SoundAsset () {
}

SoundAsset::~SoundAsset () {
}


bool SoundAsset::Load (const DataItem& item) {
    ov_callbacks vorbisCallbacks;
    vorbisCallbacks.read_func = VorbisRead;
	vorbisCallbacks.close_func = VorbisClose;
	vorbisCallbacks.seek_func = VorbisSeek;
	vorbisCallbacks.tell_func = VorbisTell;

    OggFile file;
    file.data = item.data;
    file.size = item.size;
    file.readBytes = 0;

	if (ov_open_callbacks(&file, &m_oggStream, NULL, 0, vorbisCallbacks) != 0) {
		LOG(ERROR) << "Could not read Ogg file from memory.";
        return false;
    }

    m_info = ov_info(&m_oggStream, -1);
    m_comment = ov_comment(&m_oggStream, -1);

    if(m_info->channels == 1)
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

    return true;
}

void SoundAsset::Reload () {
}

void SoundAsset::Unload () {
}

void SoundAsset::Play () {
}

bool SoundAsset::IsPlaying () {
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

/*
void ogg_stream::release()
{
    alSourceStop(source);
    empty();
    alDeleteSources(1, &source);
    check();
    alDeleteBuffers(1, buffers);
    check();

    ov_clear(&oggStream);

	// Free the memory that we created for the file
	delete[] oggMemoryFile.dataPtr;
	oggMemoryFile.dataPtr = NULL;
}

bool ogg_stream::playback()
{
    if(playing())
        return true;

    if(!stream(buffers[0]))
        return false;

    if(!stream(buffers[1]))
        return false;

    alSourceQueueBuffers(source, 2, buffers);
    alSourcePlay(source);

    return true;
}




bool ogg_stream::playing()
{
    ALenum state;

    alGetSourcei(source, AL_SOURCE_STATE, &state);

    return (state == AL_PLAYING);
}




bool ogg_stream::update()
{
    int processed;
    bool active = true;

    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

    while(processed--)
    {
        ALuint buffer;

        alSourceUnqueueBuffers(source, 1, &buffer);
        check();

        active = stream(buffer);

        alSourceQueueBuffers(source, 1, &buffer);
        check();
    }

    return active;
}




bool ogg_stream::stream(ALuint buffer)
{
    char pcm[BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;

    while(size < BUFFER_SIZE)
    {
        result = ov_read(&oggStream, pcm + size, BUFFER_SIZE - size, 0, 2, 1, &section);

        if(result > 0)
            size += result;
        else
            if(result < 0)
                throw errorString(result);
            else
                break;
    }

    if(size == 0)
        return false;

    alBufferData(buffer, format, pcm, size, vorbisInfo->rate);
    check();

    return true;
}




void ogg_stream::empty()
{
    int queued;

    alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);

    while(queued--)
    {
        ALuint buffer;

        alSourceUnqueueBuffers(source, 1, &buffer);
        check();
    }
}




void ogg_stream::check()
{
	int error = alGetError();

	if(error != AL_NO_ERROR)
		throw string("OpenAL error was raised.");
}



string ogg_stream::errorString(int code)
{
    switch(code)
    {
        case OV_EREAD:
            return string("Read from media.");
        case OV_ENOTVORBIS:
            return string("Not Vorbis data.");
        case OV_EVERSION:
            return string("Vorbis version mismatch.");
        case OV_EBADHEADER:
            return string("Invalid Vorbis header.");
        case OV_EFAULT:
            return string("Internal logic fault (bug or heap/stack corruption.");
        default:
            return string("Unknown Ogg error.");
    }
}
*/
}
