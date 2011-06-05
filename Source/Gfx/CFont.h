#pragma once

#include "GfxBase.h"
#include "Core/Core.h"

class CTexture;

class CFont {
public:
    CFont (CTexture* tex, CTexture* maskTex);
    ~CFont () {}

    // Gets properties about text in this font
    // Returns real32s in screen space
    real32 GetWidth (const char* txt);
    real32 GetHeight (const char* txt);

    void SetColor (real32 r, real32 g, real32 b);
    void SetColor (CColor color);

    void SetSize (real32 size) { m_size = size; }

    void Print (const char* txt, real32 x, real32 y);

    static void Initialize ();
    static void Terminate () { }
private:
    void DrawText (const char* text);

    C2iVector GetTextureCellCoords(char c);
    real32 GetScaledKerning(char c);

    CColor m_color;
    CTexture* m_maskTexture;
    CTexture* m_texture;

    real32 m_size;
};