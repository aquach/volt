#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/SceneHook.h"

namespace Volt {
    class Entity;
}

class GamePerfHook : public Volt::SceneHook {
public:
    GamePerfHook ();
    virtual ~GamePerfHook ();

    virtual void OnUpdateStart ();
    virtual void OnUpdateEnd ();

    virtual void OnRenderStart ();
    virtual void OnRenderEnd ();

    virtual void OnEntityRenderStart (Volt::Entity* entity);
    virtual void OnEntityRenderEnd (Volt::Entity* entity);

    virtual void OnEntityUpdateStart (Volt::Entity* entity);
    virtual void OnEntityUpdateEnd (Volt::Entity* entity);

private:
    // Maps entity tags to their total processing time in microseconds.
    typedef map<string, long> Times;
    Times m_renderTimes;
    Times m_updateTimes;
    Times m_maxRenderTimes;

    // Tags to number of entities rendered.
    typedef map<string, int> Counts;
    Counts m_renderCounts;
    Counts m_updateCounts;

    long m_usecs;
    long m_updateTotal;
    long m_renderTotal;

    long m_renderTime;
    long m_updateTime;
    long m_renderTimeTotal;
    long m_frameCount;
};
