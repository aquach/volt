#pragma once

#include <cassert>

#if defined(_MSC_VER) || defined(WIN32)
    #define COMPILER_VCC 1
    #define COMPILER_GCC 0
#elif defined(__GNUC__)
    #define COMPILER_VCC 0
    #define COMPILER_GCC 1
#endif

#ifndef NDEBUG
    #define DEBUG 1
    #define RELEASE 0
#else
    #define DEBUG 0
    #define RELEASE 1
#endif

// Disable deprecation warning.
#if COMPILER_VCC
    #pragma warning(disable: 4996)
#endif

// General type typedefs.

//typedef char int8;
typedef unsigned char uint8;

typedef short int16;
typedef unsigned short uint16;

//typedef long int int32;
//typedef unsigned long int uint32;

typedef long long int64;
typedef unsigned long long uint64;

typedef unsigned int uint;

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define FOR_(type, i, container) \
    for (type i = container.begin(); i != container.end(); ++i)
