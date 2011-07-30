#pragma once

#include "Volt/Core/Core.h"

using Volt::LogStream;
using Volt::INFO;
using Volt::WARNING;
using Volt::ERROR;
using Volt::FATAL;
using Volt::PERF;

using Volt::Vector2;

#define DEBUG_FONT_NAME "Fonts/Inconsolata.ttf"
#define DEBUG_FONT_SMALL G_AssetManager->GetFont(DEBUG_FONT_NAME, 16)
#define DEBUG_FONT G_AssetManager->GetFont(DEBUG_FONT_NAME, 32)
#define DEBUG_FONT_LARGE G_AssetManager->GetFont(DEBUG_FONT_NAME, 64)
