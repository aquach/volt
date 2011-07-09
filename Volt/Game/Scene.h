#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Graphics/Camera.h"
#include "Volt/Graphics/Input.h"
#include "Volt/Game/Game.h"
#include <set>

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

    virtual void OnBegin () { }
    virtual void OnEnd () { }
    virtual void OnPreRender () { }
    virtual void OnPostRender () { }

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

    Camera* camera () { return &m_camera; }

    void GetEntitiesAtPoint (Vector2 point, vector<Entity*>* entities);

protected:
    Game* m_game;
    bool m_isPaused;
    Camera m_camera;
    bool m_isEditor;

private:
    friend class Game;

    void ResolveEntityChanges ();
    void RemoveAll ();

    typedef map<int, list<Entity* > > Layers;
    Layers m_layers;
    set<Entity*> m_entitiesToAdd;
    set<Entity*> m_entitiesToRemove;

    list<Filter*> m_filters;

    DISALLOW_COPY_AND_ASSIGN(Scene);
};

}
