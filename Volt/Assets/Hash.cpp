#include "Volt/Assets/Hash.h"

namespace Volt {

static int c_crctab[256];
static bool initialized = false;

static void InitializeHash () {
    uint crc, poly;
    int i, j;
    poly = 0xEDB88320L;
    for  (i = 0; i < 256; i++) {
        crc = i;
        for  (j = 8; j > 0; j--) {
            if  (crc & 1) {
                crc = (crc >> 1) ^ poly;
            } else {
                crc >>= 1;
            }
        }
        c_crctab[i] = crc;
    }
}

uint HashData (void* ptr, int length) {
    if (!initialized) {
        InitializeHash();
        initialized = true;
    }

    char* block = (char*)ptr;
    uint crc = 0xFFFFFFFF;
    for (int i = 0; i < length; i++) {
        crc = ((crc >> 8) & 0x00FFFFFF) ^ c_crctab[(crc ^ *block++) & 0xFF];
    }
    return (crc ^ 0xFFFFFFFF);
}

}
