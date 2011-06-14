#include "System.h"
#include "CoreBase.h"
#include <string.h>
#if COMPILER_GCC
#include <unistd.h>
#include <sys/stat.h>
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
    return "";
#endif
}

}
