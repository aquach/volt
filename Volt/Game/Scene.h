#pragma once

#include "Core/Core.h"
#include "Graphics/Camera.h"
#include "Graphics/Input.h"

namespace Volt {

class Entity;
class Filter;
class Game;

/**
 *  A scene of the game. Scenes can be startup screens, level screens, gameplay,
    etc.
 */
class Scene {
public:
    Scene ();
    virtual ~Scene ();

    virtual void Render ();
    virtual void Update ();

    virtual void OnBegin () { };
    virtual void OnEnd () { };

    void Add (Entity* entity, int layer = 0);
    void Remove (Entity* entity);

    virtual void OnKeyEvent (SDL_KeyboardEvent event) { }
    virtual void OnMouseButtonEvent (SDL_MouseButtonEvent event) { }
    virtual void OnMouseMoveEvent (SDL_MouseMotionEvent event) { }

    int numFilters () const { return m_filters.size(); }

    /* We don't let entities and filters change their layer except when adding
     * them, because the scene's data structures need to keep this in mind. */
    void AddFilter (Filter* filter, int layer = 0);
    void RemoveFilter (Filter* filter);

private:
    friend class Game;

    void ResolveEntityChanges ();
    void RemoveAll ();

    bool m_isPaused;
    Game* m_game;
    Camera m_camera;

    typedef map<int, list<Entity* > > Layers;
    Layers m_layers;
    list<Entity*> m_entitiesToAdd;
    list<Entity*> m_entitiesToRemove;

    list<Filter*> m_filters;

    DISALLOW_COPY_AND_ASSIGN(Scene);
};

}
