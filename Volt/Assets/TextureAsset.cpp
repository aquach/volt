#include "TextureAsset.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include "AssetManager.h"
#include "DataSource.h"

namespace Volt {

bool TextureAsset::Load (const DataItem& item, FilterType filterType,
                         bool repeatX, bool repeatY) {
    SDL_RWops* rw = SDL_RWFromMem(item.data, item.size);
    SDL_Surface* tex = IMG_Load_RW(rw, 1);
    if (tex == NULL) {
        LOG(WARNING) << "Failed to load texture asset " << item.path;
        return false;
    }

    int w = tex->w;
    int h = tex->h;

    SDL_Surface* newSurface;
    bool hasAlpha = (tex->format->BytesPerPixel == 4);
    if (hasAlpha) {
        newSurface = SDL_CreateRGBSurface(0, w, h, 32, 0xff000000, 0x00ff0000,
                                          0x0000ff00, 0x000000ff);
    } else {
        newSurface = SDL_CreateRGBSurface(0, w, h, 24, 0xff000000, 0x00ff0000,
                                          0x0000ff00, 0);
    }
    SDL_BlitSurface(tex, 0, newSurface, 0);

    GLuint glTexture;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    int filterTypeParam;
    if (filterType == e_filterTypeLinear)
        filterTypeParam = GL_LINEAR;
    else if (filterType == e_filterTypeNone)
        filterTypeParam = GL_NEAREST;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterTypeParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterTypeParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    repeatX ? GL_REPEAT : GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    repeatY ? GL_REPEAT : GL_CLAMP);

    if (hasAlpha) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, newSurface->pixels);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     newSurface->pixels);
    }

    m_path = item.path;
    m_texId = glTexture;
    m_width = newSurface->w;
    m_height = newSurface->h;

    SDL_FreeSurface(newSurface);
    SDL_FreeSurface(tex);

    return true;
}

void TextureAsset::Reload () {
    Unload();
    DataItem item;
    m_manager->ReloadPath(this, &item);
    Load(item, m_filterType, m_repeatX, m_repeatY);
}

void TextureAsset::Unload () {
    glDeleteTextures(1, (GLuint*)&m_texId);
    m_texId = 0;
}

}
