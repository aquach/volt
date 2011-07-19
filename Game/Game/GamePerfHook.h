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
private:
    // Maps entity tags to their total rendering time in microseconds.
    typedef map<string, long> Times;
    Times m_times;

    // Tags to number of entities rendered.
    typedef map<string, int> Counts;
    Counts m_counts;

    long m_usecs;
    long m_total;
};
