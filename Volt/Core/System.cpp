#include "Volt/Core/System.h"
#include "Volt/Core/Core.h"
#include <string.h>

#if COMPILER_GCC
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>
#include <execinfo.h>
#include <cxxabi.h>
#endif

namespace Volt {

long GetMicroseconds () {
    int hour, min, sec;
    long usec;
    GetTimestamp(&hour, &min, &sec, &usec);
    return usec + (sec + min * 60 + hour * 3600) * 1000000L;
}

#if COMPILER_GCC

// UNIX implementations.

void GetTimestamp (int* hour, int* min, int* sec, long* usec) {
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    struct tm* tm = localtime(&tv.tv_sec);

    *hour = tm->tm_hour;
    *min = tm->tm_min;
    *sec = tm->tm_sec;
    *usec = tv.tv_usec;
}

void SleepMicroseconds (long usecs) {
    usleep(usecs);
}

string GetExecutableDirectory (string exePath) {
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
}

static vector<string>* fileList;
static string pathString;

static int FileCallback (const char* filePath, const struct stat* statb,
                         int typeFlag) {
    if (typeFlag == FTW_F)
        fileList->push_back(string(filePath).substr(pathString.size() + 1));
    return 0;
}

void GetAllFilesInDirectory (string path, vector<string>* files) {
    fileList = files;
    pathString = path;
    ftw(path.c_str(), FileCallback, 10);
    fileList = NULL;
    pathString = "";
}

string DemangleSymbol (const char* symbol) {
    size_t size;
    int status;
    char temp[128];
    char* demangled;
    //first, try to demangle a c++ name
    if (1 == sscanf(symbol, "%*[^(]%*[^_]%127[^)+]", temp)) {
        if (NULL != (demangled = abi::__cxa_demangle(temp, NULL, &size,
                                                     &status))) {
            std::string result(demangled);
            free(demangled);
            return result;
        }
    }
    //if that didn't work, try to get a regular c symbol
    if (1 == sscanf(symbol, "%127s", temp)) {
        return temp;
    }
 
    //if all else fails, just return the symbol
    return symbol;
}

void PrintStackTrace () {
    void* trace[32];
    int size = backtrace(trace, 32);
    char** strings = backtrace_symbols(trace, size);
    LOG(INFO) << "== STACK TRACE =="; 
    for (int i = 0; i < size; i++)
        LOG(INFO) << "@ " << DemangleSymbol(strings[i]);
    LOG(INFO) << "================="; 
    free(strings);
}

#else

// Windows implementations.

void GetTimestamp (int* hour, int* min, int* sec, long* usec) {
    SYSTEMTIME lt;
    GetLocalTime(&lt);

    *hour = lt.wHour;
    *min = lt.wMinute;
    *sec = lt.wSecond;
    *usec = lt.wMilliseconds * 1000;
}

void PrintStackTrace () {
}

void GetAllFilesInDirectory (string path, vector<string>* files) {
    // TODO
}

string GetExecutableDirectory (string exePath) {
    // TODO
    return "";
}

void SleepMicroseconds (long usecs) {
    Sleep(usecs / 1000);
}

#endif

}
