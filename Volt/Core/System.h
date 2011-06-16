#pragma once

#include <string>
#include <vector>

using namespace std;

namespace Volt {

void SleepMicroseconds (long usecs);

string GetExecutableDirectory (string exePath);

void GetAllFilesInDirectory (string path, vector<string>* files);

}
