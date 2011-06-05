#include "CTexture.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Resources/ResourceConstants.h"

CTextureTable             CTexture::s_textures;

#define SET_RED(P, C)   (P = (((P) & 0xffffff00) | (C)))
#define SET_GREEN(P, C) (P = (((P) & 0xffff00ff) | ((C) << 8)))
#define SET_BLUE(P, C)  (P = (((P) & 0xff00ffff) | ((C) << 16)))

inline void LoadTex (string filename, uint32*& data, uint32* width, uint32* height){
    
    filename = string(c_textureDirectory) + filename;
    SDL_Surface* tex;
    tex = IMG_Load(filename.c_str());
    if (tex == NULL)
        return;

    SDL_PixelFormat newFormat = *tex->format;
    newFormat.BytesPerPixel = sizeof(uint32);
    newFormat.BitsPerPixel = 8*sizeof(uint32);
    
    SDL_Surface* conv =  SDL_ConvertSurface(tex,&newFormat,SDL_SWSURFACE);
    SDL_FreeSurface(tex);

    
    *width = conv->w;
    *height= conv->h;

    data = new uint32[conv->w * conv->h];


    for(int r = 0; r < conv->h; r++){
        for(int c = 0; c < conv->w; c++){
            int index = (conv->h-1 -r)*conv->w + c;
            unsigned int pixel = *(((unsigned int*) conv->pixels)+r*conv->w+c);
            SET_RED( data[index], ((pixel & newFormat.Rmask) >> newFormat.Rshift) <<newFormat.Rloss);
            SET_GREEN( data[index], ((pixel & newFormat.Gmask) >> newFormat.Gshift) <<newFormat.Gloss);
            SET_BLUE( data[index], ((pixel & newFormat.Bmask) >> newFormat.Bshift) <<newFormat.Bloss);
            //Set Alpha bits
            data[index] = data[index] |  0xff000000; 
        }
    }

    SDL_FreeSurface(conv);
}

//-------------------------------------------------------------------------------------------------------
CTexture* CTexture::LoadTexture (const char* name, const char* filename) {
    uint32* data = NULL;
    uint32 height, width;

    LoadTex(filename, data, &width, &height);

    if (data == NULL) {
        // Fix: send error or something
        return NULL;
    }

    GLuint glTexture;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    CTexture* tex = new CTexture;
    tex->m_glTexture = glTexture;
    tex->m_name = name;
    tex->m_width = width;
    tex->m_height = height;
    
    delete[] data;

    return tex;
}

//-------------------------------------------------------------------------------------------------------
CTexture* CTexture::Get (const char* name) {
    CTextureTable::iterator iter = s_textures.find(name);
    if (iter == s_textures.end())
        return NULL;

    return iter->second;
}
