#pragma once

#include "Volt/Core/Core.h"

using Volt::LogStream;
using Volt::INFO;
using Volt::WARNING;
using Volt::ERROR;
using Volt::FATAL;

using Volt::Vector2;

#define DEBUG_FONT_NAME "Inconsolata.ttf"
#define DEBUG_FONT_SMALL Volt::G_AssetManager->GetFont(DEBUG_FONT_NAME, 16)
#define DEBUG_FONT Volt::G_AssetManager->GetFont(DEBUG_FONT_NAME, 32)
#define DEBUG_FONT_LARGE Volt::G_AssetManager->GetFont(DEBUG_FONT_NAME, 64)
