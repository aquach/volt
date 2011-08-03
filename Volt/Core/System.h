#pragma once

#include <string>
#include <vector>

using namespace std;

namespace Volt {

long GetMicroseconds ();

void GetTimestamp (int* hour, int* min, int* sec, long* usec);

void SleepMicroseconds (long usecs);

string GetExecutableDirectory (const string& exePath);

void GetAllFilesInDirectory (const string& path, vector<string>* files);

void PrintStackTrace ();

}
