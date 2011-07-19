#pragma once

#include "Volt/Core/Core.h"

namespace Volt {

class Entity;
class Filter;
class Scene;

/**
 *  An interface for hooking into scene methods, ie for profiling.
 */
class SceneHook {
public:
    SceneHook () { };
    virtual ~SceneHook () { };

    virtual void OnUpdateStart () { };
    virtual void OnUpdateEnd () { };

    virtual void OnRenderStart () { };
    virtual void OnRenderEnd () { };

    virtual void OnEntityRenderStart (Entity* entity) { };
    virtual void OnEntityRenderEnd (Entity* entity) { };

    virtual void OnEntityUpdateStart (Entity* entity) { };
    virtual void OnEntityUpdateEnd (Entity* entity) { };

    virtual void OnFilterRenderStart (Filter* filter) { };
    virtual void OnFilterRenderEnd (Filter* filter) { };

protected:
    friend class Scene;
    Scene* m_scene;

private:
    DISALLOW_COPY_AND_ASSIGN(SceneHook);
};

}
