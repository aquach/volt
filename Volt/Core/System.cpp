#include "System.h"
#include "CoreBase.h"
#include <string.h>
#if COMPILER_GCC
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>
#endif

namespace Volt {

string GetExecutableDirectory (string exePath) {
#if COMPILER_GCC
    char path[255];
    getcwd(path, 255);
    strcat(path, "/");
    strcat(path, exePath.c_str());

    string str(path);
    str = str.substr(0, str.rfind("/") + 1);

    struct stat st;
    if (stat(str.c_str(), &st) == 0)
        return str;

    return "";
#else
    // TODO
    return "";
#endif
}

#if COMPILER_GCC

static vector<string>* fileList;

static int FileCallback (const char* filePath, const struct stat* statb,
                         int typeFlag) {
    if (typeFlag == FTW_F)
        fileList->push_back(filePath);
    return 0;
}

void GetAllFilesInDirectory (string path, vector<string>* files) {
    fileList = files;
    ftw(path.c_str(), FileCallback, 10);
    fileList = NULL;
}

#else
void GetAllFilesInDirectory (string path, vector<string>* files) {
    // TODO
}
#endif

}
