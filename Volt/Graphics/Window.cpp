#include "Window.h"
#include "Game/Game.h"
#include "OpenGL.h"

namespace Volt {

Window* Window::instance = NULL;

Window::Window (Game* game, const string& name, int w, int h, bool fullscreen)
    : m_game(game),
      m_screen(NULL) {

    LOG(INFO) << "Initializing window...";

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
    SDL_Surface* screen = SDL_SetVideoMode(w, h, 0, flags);
    if (screen == NULL) {
        LOG(FATAL) << "Unable to set video mode: " << SDL_GetError();
        exit(1);
    }

    SDL_WM_SetCaption(name.c_str(), NULL);

    m_screen = screen;
}

Window::~Window () {
    LOG(INFO) << "Destroying window...";
    Close();
}

void Window::Close () {
    SDL_Quit();
}

void Window::Screenshot (const string& filename) {
    SDL_Surface* screen = (SDL_Surface*)m_screen;

    SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                             screen->w, screen->h, 24,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    0x000000FF, 0x0000FF00, 0x00FF0000, 0
#else
    0x00FF0000, 0x0000FF00, 0x000000FF, 0
#endif
    );

    if (temp == NULL)
        return;

    unsigned char* pixels = (unsigned char*)malloc(3 * screen->w * screen->h);

    if (pixels == NULL) {
        SDL_FreeSurface(temp);
        return;
    }

    glReadPixels(0, 0, screen->w, screen->h,
                 GL_RGB, GL_UNSIGNED_BYTE, pixels);

    for (int i = 0; i < screen->h; i++)
        memcpy(((char *)temp->pixels) + temp->pitch * i,
               pixels + 3 * screen->w * (screen->h - i - 1),
               screen->w * 3);
    free(pixels);

    SDL_SaveBMP(temp, filename.c_str());
    SDL_FreeSurface(temp);

    return;
}

int Window::width () const {
    return ((SDL_Surface*)m_screen)->w;
}

int Window::height () const {
    return ((SDL_Surface*)m_screen)->h;
}

bool Window::IsKeyPressed (SDLKey key) {
    Uint8* state = SDL_GetKeyState(NULL);
    return state[key];
}

void Window::UpdateInput () {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                m_game->OnKeyEvent(event.key);
            break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                m_game->OnMouseButtonEvent(event.button);
            break;
            case SDL_MOUSEMOTION:
                m_game->OnMouseMoveEvent(event.motion);
            break;
            case SDL_QUIT:
                m_game->Quit();
            break;
        }
    }
}

}
