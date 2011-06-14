#pragma once

#include <string>
#include <vector>

using namespace std;

namespace Volt {

string GetExecutableDirectory (string exePath);

void GetAllFilesInDirectory (string path, vector<string>* files);

}
