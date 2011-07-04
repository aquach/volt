#include "Volt/Core/Time.h"

namespace Volt {

Time::Time ()
    : started(false) {
}

void Time::Start () {
    CHECK(!started);
#if COMPILER_GCC
    gettimeofday(&startTime, NULL);
#else
    startTickCount = GetTickCount();
#endif
    started = true;
}

long Time::GetMilliseconds () const {
    CHECK(started);

#if COMPILER_GCC
    struct timeval now;
    gettimeofday(&now, NULL);
    long ticks = (now.tv_sec - startTime.tv_sec) * 1000 + (now.tv_usec - startTime.tv_usec) / 1000;
    return ticks;
#else
    long now = GetTickCount();
    long ticks = now - startTickCount;
    return ticks;
#endif
}

}
