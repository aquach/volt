#pragma once

#include "CoreBase.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

using namespace std;

namespace Volt {

enum LogType {
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4
};

#define LOG(i) \
    LogStream(i, __FILE__, __LINE__).Stream()

#define CHECK(condition) \
    LogStream(__FILE__, __LINE__, condition, #condition).Stream()

#define CHECK_OP(a, b, op) \
    LogStream(__FILE__, __LINE__, (int)a, (int)b, (a) op (b), #op).Stream()

#define CHECK_EQ(a, b) CHECK_OP(a, b, ==)
#define CHECK_GT(a, b) CHECK_OP(a, b, >)
#define CHECK_LT(a, b) CHECK_OP(a, b, <)
#define CHECK_GE(a, b) CHECK_OP(a, b, >=)
#define CHECK_LE(a, b) CHECK_OP(a, b, <=)

#define CHECK_NOTNULL(val) \
    LogStream(#val, __FILE__, __LINE__, val).Stream()

void GetTimestamp (int* hour, int* min, int* sec, long* usec);

class LogStream {
public:
    LogStream (LogType type, const char* file, int lineNumber) {
        this->type = type;
        this->checkCondition = false;
        SetupHeader(type, file, lineNumber);
    }

    LogStream (const char* file, int lineNumber, bool condition,
               const char* conditionStr) {
        this->type = FATAL;
        this->checkCondition = true;
        this->condition = condition;
        if (!condition) {
            SetupHeader(FATAL, file, lineNumber);
            os << "Check failed: " << conditionStr << " ";
        }
    }

    LogStream (const char* file, int lineNumber, int a, int b, bool condition,
               const char* op) {
        this->type = FATAL;
        this->checkCondition = true;
        this->condition = condition;
        if (!condition) {
            SetupHeader(FATAL, file, lineNumber);
            os << "Check failed: " << a << " " << op << " " << b
               << " (" << a << " vs. " << b << ") ";
        }
    }

    LogStream (const char* ptrName, const char* file, int lineNumber,
               void* ptr) {
        this->type = FATAL;
        this->checkCondition = true;
        this->condition = (ptr != NULL);
        if (!condition) {
            SetupHeader(FATAL, file, lineNumber);
            os << "Check failed: " << ptrName << " was null.";
        }
    }

    ~LogStream () {
        if (!checkCondition || !condition) {
            os << endl;
            cerr << os.str();
            if (type == FATAL)
                exit(1);
        }
    }

    ostream& Stream () { return os; }

    void SetupHeader (LogType type, const char* file, int lineNumber) {
        switch (type) {
            case INFO: os << "I "; break;
            case WARNING: os << "W "; break;
            case ERROR: os << "E "; break;
            case FATAL: os << "F "; break;
        }

        int hour = 0, min = 0, sec = 0;
        long usec = 0;
        GetTimestamp(&hour, &min, &sec, &usec);

        os << setfill('0') << setw(2) << hour
           << ":" << setw(2) << min
           << ":" << setw(2) << sec
           << "." << usec;
        os << " ";
        string fileStr(file);
        size_t index = fileStr.find_last_of("/\\");
        if (index != string::npos) {
            fileStr = fileStr.substr(index + 1);
        }
        os << fileStr << ":" << lineNumber << "] ";
    }
private:
    bool checkCondition;
    bool condition;
    stringstream os;
    LogType type;
};

}
