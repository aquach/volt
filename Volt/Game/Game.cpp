#include "Game.h"

#define MAX_DELTA_TIME (1.0f / 30.0f)

namespace Volt {

Game::Game (const string& name, int w, int h, bool fullscreen)
    : m_name(name),
      m_willQuit(false),
      m_ticksPerFrame(0),
      m_lastTick(0) {
    Random::Seed();
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

        // TODO: Do stuff

        m_lastTick = tick;
    }
}

void Game::Quit () {
    m_willQuit = true;
}

}
