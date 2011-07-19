#pragma once

#include "Game/Core/Core.h"
#include "Volt/Game/SceneHook.h"

namespace Volt {
    class Entity;
    class Filter;
    class Scene;
}

class GamePerfHook : public Volt::SceneHook {
public:
    GamePerfHook ();
    virtual ~GamePerfHook ();

    virtual void OnEntityRenderStart (Volt::Entity* entity);
    virtual void OnEntityRenderEnd (Volt::Entity* entity);

    virtual void OnEntityUpdateStart (Volt::Entity* entity);
    virtual void OnEntityUpdateEnd (Volt::Entity* entity);

private:
    // Maps entity tags to their total processing time in microseconds.
    typedef map<string, long> Times;
    Times m_renderTimes;
    Times m_updateTimes;

    // Tags to number of entities rendered.
    typedef map<string, int> Counts;
    Counts m_counts;

    long m_usecs;
    long m_updateTotal;
    long m_renderTotal;
};
