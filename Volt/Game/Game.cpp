#include "Game.h"
#include <SDL/SDL.h>
#include "Assets/AssetManager.h"
#include "Assets/DataSource.h"
#include "Graphics/Graphics.h"
#include "Graphics/Window.h"
#include "Scene.h"

#define MAX_DELTA_TIME (1.0f / 30.0f)

namespace Volt {

Game* Game::instance = NULL;

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

    m_window = new Window(this, name, w, h, fullscreen);
    Window::Register(m_window);

    m_graphics = new Graphics(m_window);
    m_graphics->Set2D(w, h);
    m_graphics->Init();

    m_assetManager = new AssetManager(source);
    AssetManager::Register(m_assetManager);

    Game::Register(this);
}

Game::~Game () {
    if (m_currentScene != NULL) {
        m_currentScene->OnEnd();
        delete m_currentScene;
    }
    delete m_assetManager;
    delete m_window;
}

void Game::Run () {
    LOG(INFO) << "Starting game...";

    m_time.Start();

    m_lastTick = m_time.GetMilliseconds();

    while (true) {
        if (m_willQuit)
            break;

        long tick = m_time.GetMilliseconds();

        long delta = tick - m_lastTick;

        m_ticksPerFrame = m_ticksPerFrame * 0.9f + delta * 0.1f;

        m_dt = MIN((tick - m_lastTick) * 0.001f, MAX_DELTA_TIME);

        m_window->UpdateInput();

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

void Game::OnKeyEvent (SDL_KeyboardEvent event) {
    if (m_currentScene != NULL)
        m_currentScene->OnKeyEvent(event);
}

void Game::OnMouseButtonEvent (SDL_MouseButtonEvent event) {
    if (m_currentScene != NULL)
        m_currentScene->OnMouseButtonEvent(event);
}

void Game::OnMouseMoveEvent (SDL_MouseMotionEvent event) {
    if (m_currentScene != NULL)
        m_currentScene->OnMouseMoveEvent(event);
}

void Game::Quit () {
    m_willQuit = true;
}

}
