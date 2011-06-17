#pragma once

#include <cstdio>
#include <cstring>
#include <ctime>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "BBox.h"
#include "Color.h"
#include "CoreBase.h"
#include "Macros.h"
#include "Math.h"
#include "Random.h"
#include "System.h"
#include "Time.h"
#include "Transform.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Logging.h"

#if COMPILER_VCC
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace std;
