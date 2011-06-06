#pragma once

#include "Core/Core.h"

#ifdef COMPILER_GCC
    #include <sys/time.h>
#else
    // TODO
#endif

namespace Volt {

class Time {
public:
    Time ();
    void Start ();

    long GetMilliseconds () const;
private:
    bool started;
#ifdef COMPILER_GCC
    timeval startTime;
#else
    // TODO
#endif
};

}
