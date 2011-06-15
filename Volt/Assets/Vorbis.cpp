#include "Vorbis.h"

namespace Volt {

size_t VorbisRead (void *ptr, size_t byteSize, size_t sizeToRead,
                   void *datasource) {
    size_t spaceToEOF;
    size_t actualSizeToRead;
    OggFile* vorbisData;

    vorbisData = (OggFile*)datasource;

    spaceToEOF = vorbisData->size - vorbisData->readBytes;
    if ((sizeToRead*byteSize) < spaceToEOF)
        actualSizeToRead = (sizeToRead*byteSize);
    else
        actualSizeToRead = spaceToEOF;

    if (actualSizeToRead) {
        memcpy(ptr, (char*)vorbisData->data + vorbisData->readBytes, actualSizeToRead);
        vorbisData->readBytes += (actualSizeToRead);
    }

    return actualSizeToRead;
}

int VorbisSeek (void* datasource, ogg_int64_t offset, int whence) {
    size_t spaceToEOF;
    ogg_int64_t actualOffset;
    OggFile* vorbisData = (OggFile*)datasource;
    switch (whence) {
        case SEEK_SET:
            if (vorbisData->size >= offset)
                actualOffset = offset;
            else
                actualOffset = vorbisData->size;
            vorbisData->readBytes = (int)actualOffset;
        break;
        case SEEK_CUR:
            spaceToEOF = vorbisData->size - vorbisData->readBytes;
            if (offset < spaceToEOF)
                actualOffset = (offset);
            else
                actualOffset = spaceToEOF;
            vorbisData->readBytes += actualOffset;
        break;
        case SEEK_END:
            vorbisData->readBytes = vorbisData->size+1;
        break;
        default:
            LOG(ERROR) << "Unknown seek command in VorbisSeek.";
        break;
    };

    return 0;
}

int VorbisClose (void* datasource) {
    return 1;
}

long VorbisTell (void *datasource) {
    OggFile* vorbisData = (OggFile*)datasource;
    return vorbisData->readBytes;
}

}
