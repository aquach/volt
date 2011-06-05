#pragma once

#include "Core/Core.h"

// Directory for shaders
const char* const c_windowTitle = "Facebook Graph";
const real32 c_framesPerSecond = 30;
const real32 c_secondsPerFrame = 1.0f / c_framesPerSecond;
const uint32 c_windowWidth = 1024;
const uint32 c_windowHeight = 768;
//const uint32 c_windowWidth = 1440;
//const uint32 c_windowHeight = 900;
const real32 c_aspectRatio = (real32)c_windowWidth / c_windowHeight;
const real32 c_nearPlane = 1.0f;
const real32 c_farPlane = 200.0f;
const real32 c_fov = 45.0f;