#include "Window.h"
#include <SDL/SDL.h>

namespace Volt {

Window::Window (const string& name, int w, int h, bool fullscreen)
    : m_screen(NULL) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        LOG(FATAL) << "Unable to initialize SDL: " << SDL_GetError();
        exit(1);
    }

    SDL_ShowCursor(SDL_DISABLE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    int flags = SDL_SWSURFACE | SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0);
    SDL_Surface* screen = (SDL_Surface*)SDL_SetVideoMode(w, h, 0, flags);
    if (screen == NULL) {
        LOG(FATAL) << "Unable to set video mode: " << SDL_GetError();
        exit(1);
    }

    SDL_WM_SetCaption(name.c_str(), NULL);

    m_screen = screen;
}

void Window::Close () {
    SDL_Quit();
}

}
