#include "Logging.h"

#if COMPILER_GCC
    #include <sys/time.h>
#else
    // TODO
#endif

namespace Volt {

void GetTimestamp (int* hour, int* min, int* sec, long* usec) {
#if COMPILER_GCC
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    struct tm* tm = localtime(&tv.tv_sec);

    *hour = tm->tm_hour;
    *min = tm->tm_min;
    *sec = tm->tm_sec;
    *usec = tv.tv_usec;
#else
    // TODO
#endif
}

}
