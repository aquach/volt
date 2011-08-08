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
class SceneHook;

/**
 *  A scene of the game. Scenes can be startup screens, level screens, gameplay,
    etc.
 */
class Scene {
public:
    /* A listener that can be registered with the Scene to get notifications
     * when entities are added/removed. */
    class SceneListener {
    public:
        virtual void OnEntityAdded (Entity* e) { }
        virtual void OnEntityRemoved (Entity* e) { }
    };

    typedef map<int, list<Entity* > > Layers;

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
    void RemoveAll ();

    /* Called by Entities to tell the scene to update its state. */
    void OnEntityLayerChange (Entity* entity, int oldLayer, int newLayer);
    void OnEntityTagAdd (Entity* entity, const string& tag);
    void OnEntityTagRemove (Entity* entity, const string& tag);

    virtual void OnKeyEvent (SDL_KeyboardEvent event) { }
    virtual void OnMouseButtonEvent (SDL_MouseButtonEvent event) { }
    virtual void OnMouseMoveEvent (SDL_MouseMotionEvent event) { }

    int numFilters () const { return m_bottomFilters.size(); }

    void AddFilter (Filter* filter);
    void RemoveFilter (Filter* filter);
    Filter* GetFilter (const string& filterName);

    Camera* camera () { return &m_camera; }

    void GetEntitiesAtPoint (Vector2 point, vector<Entity*>* entities);
    void GetEntitiesInArea (Vector2 point1, Vector2 point2,
                            vector<Entity*>* entities);

    /* Returns a map from layers to the number of entities on that layer. */
    void GetLayerEntityCounts (map<int, int>* mapOut);

    Layers GetEntities () const { return m_layers; }
    void GetEntities (vector<Entity*>* entities) const;

    /* Sets an object to receive notifications during Scene processing for
     * performance or other purposes. */
    void SetHook (SceneHook* hook);

    void AddSceneListener (SceneListener* listener) {
        m_sceneListeners.insert(listener);
    }
    void RemoveSceneListener (SceneListener* listener) {
        m_sceneListeners.erase(listener);
    }

    Entity* GetFirstTagged (const string& tag);
    void GetAllTagged (const string& tag, vector<Entity*>* entities);

protected:
    Game* m_game;
    bool m_isPaused;
    Camera m_camera;
    bool m_isEditor;

private:
    friend class Game;

    void ResolveEntityChanges ();
    void ResolveFilterChanges ();
    void NotifyAddListeners (Entity* entity);
    void NotifyRemoveListeners (Entity* entity);

    Layers m_layers;
    set<Entity*> m_entitiesToAdd;
    set<Entity*> m_entitiesToRemove;

    typedef map<string, list<Entity*> > TagMap;
    TagMap m_entityTags;

    typedef map<int, list<Filter*> > Filters;
    Filters m_bottomFilters;
    Filters m_topFilters;
    set<Filter*> m_filtersToAdd;
    set<Filter*> m_filtersToRemove;

    SceneHook* m_hook;
    set<SceneListener*> m_sceneListeners;

    DISALLOW_COPY_AND_ASSIGN(Scene);
};

}
