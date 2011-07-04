#pragma once

#include "Volt/Core/Core.h"

#if COMPILER_GCC
    #include <sys/time.h>
#else
    #include <windows.h>
#endif

namespace Volt {

class Time {
public:
    Time ();
    void Start ();

    long GetMilliseconds () const;
private:
    bool started;
#if COMPILER_GCC
    timeval startTime;
#else
    long startTickCount;
#endif
};

}
