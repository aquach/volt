#include "Resources/CTexture.h"
#include "CFont.h"
#include "FontConstants.h"

//-------------------------------------------------------------------------------------------------------
CFont::CFont (CTexture* tex, CTexture* maskTex) :
    m_color(255, 255, 255),
    m_texture(tex),
    m_maskTexture(maskTex),
    m_size(1.0f) { }

//-------------------------------------------------------------------------------------------------------
real32 CFont::GetWidth (const char* text) {
    // Fix: Doesn't take newlines into account

    real32 widthScale = c_widthPerCharacter * m_size;

    real32 posx = 0;
    for (int i = 0; text[i] != 0; i++) {
        if (text[i] < 0 || text[i] > 256)
            continue;

        if (text[i] != ' ')
            posx += widthScale - widthScale * GetScaledKerning(text[i]);
        else
            posx += widthScale * c_spaceWidth;
    }

    return posx;
}

//-------------------------------------------------------------------------------------------------------
real32 CFont::GetHeight (const char* text) {
    int numLines = 1;
    for (int i = 0; text[i] != 0; i++)
        if (text[i] == '\n')
            ++numLines;

    return numLines * c_heightPerLine;
}

//-------------------------------------------------------------------------------------------------------
void CFont::SetColor (real32 r, real32 g, real32 b) {
    m_color.Set(r, g, b);
}

//-------------------------------------------------------------------------------------------------------
void CFont::DrawText (const char* text) {
    real32 posX = 0;
    real32 posY = 0;

    real32 widthScale = c_widthPerCharacter * m_size;
    real32 heightScale = c_heightPerLine * m_size;

    glBegin(GL_QUADS);
    for (int i = 0; text[i] != 0; i++) {
        if (text[i] == '\n') {
            posY += c_heightPerLine;
            posX = 0;
            continue;
        }

        if (text[i] < 0 || text[i] > 256)
            continue;

        if (text[i] != ' ') {
            C2iVector coords = GetTextureCellCoords(text[i]);

            glTexCoord2f( coords.x * c_cellToCellWidth + c_cellWidth, coords.y * c_cellToCellHeight);
            glVertex2f(   posX + widthScale,                 posY);

            glTexCoord2f( coords.x * c_cellToCellWidth + c_cellWidth, coords.y * c_cellToCellHeight + c_cellHeight);
            glVertex2f(   posX + widthScale,                 posY + heightScale);

            glTexCoord2f( coords.x * c_cellToCellWidth,               coords.y * c_cellToCellHeight + c_cellHeight);
            glVertex2f(   posX,                                       posY + heightScale);

            glTexCoord2f( coords.x * c_cellToCellWidth,               coords.y * c_cellToCellHeight);
            glVertex2f(   posX,                                       posY);

            posX += widthScale - widthScale * GetScaledKerning(text[i]);
        }
        else {
            posX += widthScale * c_spaceWidth;
        }
    }
    glEnd();
}

//-------------------------------------------------------------------------------------------------------
void CFont::Print (const char* text, real32 x, real32 y) {

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(x + c_renderOffsetX, y + c_renderOffsetY, 0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    // Draw mask - areas that are black on mask get blacked out
    glBlendFunc(GL_DST_COLOR, GL_ZERO);
    glBindTexture(GL_TEXTURE_2D, m_maskTexture->GetGLTexture());
    glColor3f(1, 1, 1);
    DrawText(text);

    // Draw text - adds color onto black areas
    glBindTexture(GL_TEXTURE_2D, m_texture->GetGLTexture());
    glBlendFunc(GL_ONE, GL_ONE);
    C3Vector realRGB;
    m_color.ToC3Vector(realRGB);
    glColor3f(realRGB.x, realRGB.y, realRGB.z);
    DrawText(text);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

//-------------------------------------------------------------------------------------------------------
void CFont::Initialize () {
    // Initialize kerning values

    for(int i = 0; i < 256; i++)
        c_kern[i] = 0.02f;

    for (int i = 0; i < 10; i++)
        c_kern['0' + i] = 0.01f;

    c_kern['I'] = 0.06f;
    c_kern['L'] = 0.03f;
    c_kern['E'] = 0.035f;
    c_kern['F'] = 0.035f;
    c_kern['S'] = 0.03f;
    c_kern['T'] = 0.03f;
    c_kern['P'] = 0.03f;
    c_kern['M'] = 0.015f;
    c_kern['W'] = 0.0f;

    c_kern[' '] = 0.06f;

    c_kern['i'] = 0.04f;
    c_kern['l'] = 0.04f;
    c_kern['!'] = 0.06f;
    c_kern[','] = 0.06f;
    c_kern['\''] = 0.06f;
    c_kern[':'] = 0.06f;
    c_kern['f'] = 0.05f;
    c_kern['j'] = 0.05f;
    c_kern['t'] = 0.03f;
    c_kern['e'] = 0.025f;
    c_kern['r'] = 0.025f;
    c_kern['x'] = 0.02f;
    c_kern['m'] = 0.005f;
    c_kern['w'] = 0.005f;
}

//-------------------------------------------------------------------------------------------------------
C2iVector CFont::GetTextureCellCoords (char c) {
    int num = (int)c;
    if (num < 0 || num >= 256)
        num = ' ';

    int col = num % c_cellTextureColumns;
    int row = c_cellTextureRows - num / c_cellTextureColumns - 1;
    Assert_(row < (int)c_cellTextureRows);

    return C2iVector(col, row);
}

//-------------------------------------------------------------------------------------------------------
real32 CFont::GetScaledKerning (char c) {
    return c_kern[(int)c] * c_kerningScale;
}
