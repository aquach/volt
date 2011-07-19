#include "Volt/Assets/TextureAsset.h"
#include <SDL/SDL_image.h>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Assets/DataSource.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"

namespace Volt {

TextureAsset::~TextureAsset () {
    Unload();
}

bool TextureAsset::Load (const DataItem& item, FilterType filterType,
                         bool repeatX, bool repeatY) {
    CHECK(Graphics::initialized())
        << "Graphics must be initialized before loading textures.";

    SDL_RWops* rw = SDL_RWFromMem(item.data, item.size);
    SDL_Surface* tex = IMG_Load_RW(rw, 1);
    if (tex == NULL) {
        LOG(ERROR) << "Failed to load texture asset " << item.path;
        return false;
    }

    int w = tex->w;
    int h = tex->h;

    SDL_Surface* newSurface;
    bool hasAlpha = (tex->format->BytesPerPixel == 4);
    if (hasAlpha) {
        newSurface = SDL_CreateRGBSurface(0, w, h, 32,
                                          0x000000ff,
                                          0x0000ff00,
                                          0x00ff0000,
                                          0xff000000);
        SDL_SetAlpha(tex, 0, 0);
    } else {
        newSurface = SDL_CreateRGBSurface(0, w, h, 24,
                                          0xff000000,
                                          0x0000ff00,
                                          0x00ff0000,
                                          0);
    }
    SDL_BlitSurface(tex, 0, newSurface, 0);

    GLuint glTexture;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    int filterTypeParam;
    if (filterType == FILTER_LINEAR) {
        filterTypeParam = GL_LINEAR;
    } else if (filterType == FILTER_NONE) {
        filterTypeParam = GL_NEAREST;
    } else {
        LOG(WARNING) << "Unknown filter type " << filterType;
        filterTypeParam = GL_NEAREST;
    }


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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, newSurface->pixels);
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
