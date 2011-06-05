#include "GfxHelper.h"
#include "GfxBase.h"

int Screenshot(SDL_Surface *screen, const char *filename)
{
        SDL_Surface *temp;
        unsigned char *pixels;
        int i;

        temp = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                    screen->w, screen->h, 24,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
        0x000000FF, 0x0000FF00, 0x00FF0000, 0
#else
        0x00FF0000, 0x0000FF00, 0x000000FF, 0
#endif
        );
        if (temp == NULL) return -1;

        pixels = (unsigned char*)malloc(3 * screen->w * screen->h);
        if (pixels == NULL) {
                SDL_FreeSurface(temp);
                return -1;
        }

        glReadPixels(0, 0, screen->w, screen->h,
                     GL_RGB, GL_UNSIGNED_BYTE, pixels);

        for (i=0; i<screen->h; i++)
                memcpy(((char *) temp->pixels) + temp->pitch * i,
                       pixels + 3*screen->w * (screen->h-i-1),
                       screen->w*3);
        free(pixels);

        SDL_SaveBMP(temp, filename);
        SDL_FreeSurface(temp);
        return 0;
}
