#include "Game.h"
#include <SDL/SDL.h>
#include "Assets/AssetManager.h"
#include "Assets/DataSource.h"
#include "Graphics/Window.h"
#include "Scene.h"

#define MAX_DELTA_TIME (1.0f / 30.0f)

namespace Volt {

Game::Game (const string& name, const DataSource* source, int w, int h,
            bool fullscreen)
    : m_name(name),
      m_willQuit(false),
      m_ticksPerFrame(0),
      m_lastTick(0),
      m_window(NULL),
      m_currentScene(NULL),
      m_switchToScene(NULL),
      m_assetManager(NULL) {

    Random::Seed();

    m_window = new Window(name, w, h, fullscreen);
    m_assetManager = new AssetManager(source);
}

Game::~Game () {
    if (m_currentScene != NULL) {
        m_currentScene->OnEnd();
        delete m_currentScene;
    }
    delete m_assetManager;
    delete m_window;
}

void Game::UpdateInput () { // TODO: move to Window?
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                //grapher->HandleKeyboard(event.key);
            break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                //grapher->HandleMouseButton(event.button);
            break;
            case SDL_MOUSEMOTION:
                //grapher->HandleMouseMove(event.motion);
            break;
            case SDL_QUIT:
                Quit();
            break;
        }
    }
}

void Game::Run () {
    m_time.Start();

    m_lastTick = m_time.GetMilliseconds();

    while (true) {
        if (m_willQuit)
            break;

        long tick = m_time.GetMilliseconds();

        long delta = tick - m_lastTick;

        m_ticksPerFrame = m_ticksPerFrame * 0.9 + delta * 0.1;

        m_dt = MIN(((double)(tick - m_lastTick)) * 0.001f, MAX_DELTA_TIME);

        UpdateInput();

        if (m_currentScene != NULL) {
            m_currentScene->Update();
            m_currentScene->Render();
        }

        // Swap scenes.
        if (m_switchToScene != NULL) {
            Scene* oldScene = m_currentScene;
            oldScene->OnEnd();
            oldScene->m_game = NULL;

            m_currentScene = m_switchToScene;
            m_switchToScene = NULL;
            m_currentScene->m_game = this;
            m_currentScene->OnBegin();
        }

        m_lastTick = tick;
    }

    m_window->Close();
}

void Game::SetScene (Scene* scene) {
    if (m_currentScene != NULL) {
        m_switchToScene = scene;
    } else {
        m_currentScene = scene;
        m_currentScene->m_game = this;
        m_currentScene->OnBegin();
    }
}

void Game::Quit () {
    m_willQuit = true;
}

}
