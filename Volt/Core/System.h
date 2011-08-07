#pragma once

#include <string>
#include <vector>

#include "CoreBase.h"

#if COMPILER_VCC
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef MessageBox
#endif

using namespace std;

namespace Volt {

long GetMicroseconds ();

void GetTimestamp (int* hour, int* min, int* sec, long* usec);

void SleepMicroseconds (long usecs);

string GetExecutableDirectory (const string& exePath);

void GetAllFilesInDirectory (const string& path, vector<string>* files);

void PrintStackTrace ();

#if COMPILER_GCC
    typedef pthread_mutex_t Lock;
#elif COMPILER_VCC
    typedef CRITICAL_SECTION Lock;
#endif

void InitializeLock (Lock* lock);
void DestroyLock (Lock* lock);
void AcquireLock (Lock* lock);
void ReleaseLock (Lock* lock);

}
