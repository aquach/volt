#pragma once

#include <cstdio>
#include <cstring>
#include <ctime>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Volt/Core/CoreBase.h"

#if COMPILER_VCC
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef MessageBox
#endif

#include <json/json.h>

#include "Volt/Core/BBox.h"
#include "Volt/Core/Color.h"
#include "Volt/Core/Macros.h"
#include "Volt/Core/Math.h"
#include "Volt/Core/Random.h"
#include "Volt/Core/System.h"
#include "Volt/Core/Time.h"
#include "Volt/Core/Transform.h"
#include "Volt/Core/Vector2.h"
#include "Volt/Core/Vector3.h"
#include "Volt/Core/Logging.h"

using namespace std;
