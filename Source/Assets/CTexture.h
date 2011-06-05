#pragma once

#include "Core/Core.h"
#include "Gfx/GfxBase.h"

namespace NResource {
    void LoadTextures ();
    void UnloadTextures ();
}

class CTexture;
typedef std::map<const char*, CTexture*> CTextureTable;

class CTexture {
public:
    GLuint GetGLTexture() { return m_glTexture; }

    static CTexture* LoadTexture (const char* name, const char* filename);

    static void Initialize () {
        NResource::LoadTextures();
    }
    static void Terminate () {
        NResource::UnloadTextures();
    }

    static CTexture* Get (const char* name);

    uint32 GetWidth () { return m_width; }
    uint32 GetHeight () { return m_height; }

private:
    CTexture () : m_glTexture(-1), m_width(-1), m_height(-1) { }
    ~CTexture () { glDeleteTextures(1, &m_glTexture); }

    std::string                       m_name;
    GLuint                            m_glTexture;
    uint32                            m_width;
    uint32                            m_height;

    static CTextureTable              s_textures;

    friend void NResource::LoadTextures ();
    friend void NResource::UnloadTextures ();
};