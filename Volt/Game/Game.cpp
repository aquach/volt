#include "Volt/Game/Game.h"
#include <Python.h>
#include <SDL/SDL.h>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Assets/DataSource.h"
#include "Volt/Assets/SoundManager.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/SDLWindow.h"
#include "Volt/Game/KeyBindings.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Game/Scene.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Python/Python.h"

#define MIN_DELTA_TIME (1.0f / 60.0f)
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
      m_assetManager(NULL),
      m_soundManager(NULL),
      m_physicsManager(NULL),
      m_bindings(NULL),
      m_appTime(NULL),
      m_frameNumber(0) {

    Random::Seed();

    m_appTime = new AppTime;
    AppTime::Register(m_appTime);

    m_window = new SDLWindow(this, name, w, h, fullscreen);
    Viewport::Register(m_window);

    m_graphics = new Graphics(m_window);
    m_graphics->Set2D(w, h);
    m_graphics->Init();

    m_assetManager = new AssetManager(source);
    AssetManager::Register(m_assetManager);

    m_soundManager = new SoundManager;
    SoundManager::Register(m_soundManager);

    m_physicsManager = new PhysicsManager;
    m_physicsManager->m_game = this;
    PhysicsManager::Register(m_physicsManager);

    m_bindings = new KeyBindings();
    KeyBindings::Register(m_bindings);

    Game::Register(this);

    #if DEBUG
        Graphics::CheckErrors();
        Graphics::CheckState();
    #endif
}

Game::~Game () {
    LOG(INFO) << "Closing game...";
    if (m_currentScene != NULL) {
        m_currentScene->OnEnd();
        delete m_currentScene;
    }
    delete m_bindings;
    delete m_assetManager;
    delete m_soundManager;
    delete m_window;
    delete m_appTime;
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
        if (delta < MIN_DELTA_TIME * 1000) {
            long sleepTicks = (int)(MIN_DELTA_TIME * 1000) - delta;
            Py_BEGIN_ALLOW_THREADS
            SleepMicroseconds(sleepTicks * 1000);
            Py_END_ALLOW_THREADS
            tick = m_time.GetMilliseconds();
            delta = tick - m_lastTick;
        }

        float seconds = (tick - m_lastTick) * 0.001f;
        m_ticksPerFrame = m_ticksPerFrame * 0.95f + delta * 0.05f;
        m_appTime->SetDt(MIN(seconds, MAX_DELTA_TIME));
        m_frameNumber++;

        m_window->UpdateInput();
        m_soundManager->Update();

        Py_BEGIN_ALLOW_THREADS
        SleepMicroseconds(1000);
        Py_END_ALLOW_THREADS

        m_physicsManager->Update();

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
