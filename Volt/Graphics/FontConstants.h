#pragma once

// Distance from one cell to the next (texture space)
const real32 c_cellToCellWidth = 0.0625f; 
const real32 c_cellToCellHeight = 0.0625f;

// Cell widths and heights (texture space)
const real32 c_cellWidth = 0.0625f;
const real32 c_cellHeight = 0.0625f;

// How many rows and columns are in the character sheet
const uint32 c_cellTextureRows = 16;
const uint32 c_cellTextureColumns = 16;

// Distance from target point to actually start rendering (screen space)
const real32 c_renderOffsetX = 0.0f;
const real32 c_renderOffsetY = -0.02f;

// Distances from one character to the next (screen space)
const real32 c_widthPerCharacter = 0.0825f;
const real32 c_heightPerLine = 0.14f;

// Scale to multiply kerning by for kerning
const real32 c_kerningScale = 14.0f;

// How much of a character width to use for space
const real32 c_spaceWidth = 0.3f;

// Initialized in CFont::Initialize()
// Change it there if necessary
real32 c_kern[256];