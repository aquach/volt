#pragma once

#include <cmath>

#ifndef INFINITY
    #define INFINITY 1e+08
#endif

namespace Volt {

const float PI = 3.14159265f;
const float E = 2.71828182845f;

const float DEG2RAD = 3.14159f / 180;
const float RAD2DEG = 1 / DEG2RAD;

float RoundToNearest (float number, float divisor);

}
