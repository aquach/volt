#include "Time.h"

namespace Volt {

Time::Time ()
    : started(false) {
}

void Time::Start () {
    CHECK(!started);
    gettimeofday(&startTime, NULL);
    started = true;
}

long Time::GetMilliseconds () const {
    CHECK(started);

#if COMPILER_GCC
    long ticks;
    struct timeval now;
    gettimeofday(&now, NULL);
    ticks = (now.tv_sec - startTime.tv_sec) * 1000 + (now.tv_usec - startTime.tv_usec) / 1000;
    return ticks;
#else
    // TODO
#endif
}

}
