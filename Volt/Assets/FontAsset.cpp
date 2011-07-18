#include "Volt/Assets/FontAsset.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Assets/DataSource.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"

#define STB_TRUETYPE_IMPLEMENTATION
#if COMPILER_GCC
    #pragma GCC diagnostic ignored "-Wwrite-strings"
#endif
#include <stb/stb_truetype.h>
#if COMPILER_GCC
#pragma GCC diagnostic warning "-Wwrite-strings"
#endif

#define NUM_CHARACTERS 96

namespace Volt {

FontAsset::FontAsset (int textureWidth, int textureHeight)
    : m_fontCData(NULL),
      m_textureWidth(textureWidth),
      m_textureHeight(textureHeight),
      m_texID(0),
      m_size(0) {
}

FontAsset::~FontAsset () {
    Unload();
}

bool FontAsset::Load (const DataItem& item, float size) {
    CHECK(Graphics::initialized())
        << "Graphics must be initialized before loading font.";
        
    m_size = size;
    m_path = item.path;

    m_fontCData = new stbtt_bakedchar[NUM_CHARACTERS];

    unsigned char* tempBitmap = new unsigned char[
    m_textureWidth * m_textureHeight];

    int result = stbtt_BakeFontBitmap((const unsigned char*)item.data,
                                      0, // Offset.
                                      size,
                                      tempBitmap,
                                      m_textureWidth,
                                      m_textureHeight,
                                      32, // Starting character.
                                      NUM_CHARACTERS,
                                      (stbtt_bakedchar*)m_fontCData);
    if (result <= 0) {
        LOG(WARNING) << "Could only fit " << -result << "/" << NUM_CHARACTERS
                 << " characters into bitmap.";
    }

    glGenTextures(1, (GLuint*)&m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_textureWidth, m_textureHeight,
         0, GL_ALPHA, GL_UNSIGNED_BYTE, tempBitmap);

    delete[] tempBitmap;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    LOG(INFO) << m_texID;

    return true;
}

void FontAsset::Reload () {
    Unload();
    DataItem item;
    m_manager->ReloadPath(this, &item);
    Load(item, m_size);
}

void FontAsset::Unload () {
    glDeleteTextures(1, (GLuint*)&m_texID);

    if (m_fontCData) {
        delete[] (stbtt_bakedchar*)m_fontCData;
        m_fontCData = NULL;
    }
}

float FontAsset::GetTextWidth (const string& text) {
    float width = 0;
    float x;
    float y;
    for (uint i = 0; i < text.size(); i++) {
        BBox verts;
        BBox texCoords;
        GetGlyphData(text[i], &x, &y, &verts, &texCoords);
        width = verts.max.x;
    }
    return width;
}

float FontAsset::GetTextHeight (const string& text) {
    float top = 0;
    float bottom = 0;
    for (uint i = 0; i < text.size(); i++) {
        float x = 0;
        float y = 0;
        BBox verts;
        BBox texCoords;
        GetGlyphData(text[i], &x, &y, &verts, &texCoords);
        top = MIN(verts.min.y, top);
        bottom = MAX(verts.max.y, bottom);
    }
    return bottom - top;
}

void FontAsset::GetGlyphData (char c, float* x, float* y, BBox* verts,
                  BBox* texCoords) const {
    stbtt_aligned_quad q;
    stbtt_GetBakedQuad((stbtt_bakedchar*)m_fontCData, m_textureWidth,
                       m_textureHeight,
                       c - 32, // Character index.
                       x,
                       y, // Pointer to x and y position in screen pixel space.
                       &q, // Output quad to draw.
                       1 // OpenGL Mode
                       );

    verts->min.x = q.x0;
    verts->min.y = q.y0;
    verts->max.x = q.x1;
    verts->max.y = q.y1;

    texCoords->min.x = q.s0;
    texCoords->min.y = q.t0;
    texCoords->max.x = q.s1;
    texCoords->max.y = q.t1;
}

}
