#pragma once

#include "Volt/Core/Core.h"
#include <ogg/ogg.h>

namespace Volt {

struct OggFile {
    char* data;
    int size;
    int readBytes;
};

int VorbisSeek (void* datasource, ogg_int64_t offset, int whence);

size_t VorbisRead (void* ptr, size_t byteSize, size_t sizeToRead,
                   void* datasource);

int VorbisClose (void* datasource);

long VorbisTell (void* datasource);
};
