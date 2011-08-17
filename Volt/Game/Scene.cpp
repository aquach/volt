#include "Volt/Game/Scene.h"
#include <algorithm>
#include "Volt/Game/Entity.h"
#include "Volt/Graphics/Filter.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Game/SceneHook.h"

namespace Volt {

Scene::Scene ()
    : m_isPaused(false),
      m_isEditor(false),
      m_hook(NULL) {
}

Scene::~Scene () {
    RemoveAll();
    ResolveEntityChanges();
    FOR_ (Filters::iterator, i, m_bottomFilters) {
        FOR_(list<Filter*>::iterator, filterIter, i->second)
            delete *filterIter;
    }
    if (m_hook != NULL)
        delete m_hook;
}

void Scene::RemoveAll () {
    FOR_(Layers::iterator, layer, m_layers) {
        list<Entity*>& entityList = layer->second;
        m_entitiesToRemove.insert(entityList.begin(), entityList.end());
    }
}

void Scene::Update () {
    if (m_hook != NULL)
        m_hook->OnUpdateStart();
        
    if (!m_isPaused) {
        m_camera.Update();
        FOR_ (Layers::iterator, layer, m_layers) {
            FOR_(list<Entity*>::iterator, i, layer->second) {
                if (m_hook != NULL)
                    m_hook->OnEntityUpdateStart(*i);
                (*i)->Update();
                if (m_hook != NULL)
                    m_hook->OnEntityUpdateEnd(*i);
            }
        }
    }

    ResolveEntityChanges();
    ResolveFilterChanges();

    if (m_hook != NULL)
        m_hook->OnUpdateEnd();
}

void Scene::Add (Entity* entity, int layer) {
    entity->m_layer = layer;
    m_entitiesToAdd.insert(entity);
}

void Scene::Remove (Entity* entity) {
    m_entitiesToRemove.insert(entity);
}

void Scene::NotifyAddListeners (Entity* entity) {
    FOR_(set<SceneListener*>::iterator, i, m_sceneListeners) {
        (*i)->OnEntityAdded(entity);
    }
}

void Scene::NotifyRemoveListeners (Entity* entity) {
    FOR_(set<SceneListener*>::iterator, i, m_sceneListeners) {
        (*i)->OnEntityRemoved(entity);
    }
}

void Scene::ResolveEntityChanges () {
    while (true) {
        // OnRemoved may remove other entities, so don't try to iterate.
        set<Entity*>::iterator i = m_entitiesToRemove.begin();
        if (i == m_entitiesToRemove.end())
            break;

        Entity* entity = *i;

        Layers::iterator found = m_layers.find(entity->layer());
        if (found != m_layers.end()) {
            list<Entity*>& entityList = found->second;
            list<Entity*>::iterator entityI = find(entityList.begin(),
                                                   entityList.end(),
                                                   entity);
            if (entityI != entityList.end()) {
                entityList.erase(entityI);
                NotifyRemoveListeners(entity);
                entity->OnRemoved();
                entity->m_scene = NULL;
                vector<string> tags;
                entity->GetTags(&tags);
                for (uint i = 0; i < tags.size(); i++)
                    m_entityTags[tags[i]].remove(entity);
                delete entity;
            }
        }
        m_entitiesToRemove.erase(i);
    }
    m_entitiesToRemove.clear();

    while (true) {
        // OnAdded may added other entities, so don't try to iterate.
        set<Entity*>::iterator i = m_entitiesToAdd.begin();
        if (i == m_entitiesToAdd.end())
            break;
        Entity* entity = *i;

        m_layers[entity->layer()].push_back(entity);
        entity->m_scene = this;
        vector<string> tags;
        entity->GetTags(&tags);
        for (uint i = 0; i < tags.size(); i++)
            m_entityTags[tags[i]].push_back(entity);
        entity->OnAdded();
        NotifyAddListeners(entity);
        m_entitiesToAdd.erase(i);
    }

    m_entitiesToAdd.clear();
}

void Scene::Render () {
    if (m_hook != NULL)
        m_hook->OnRenderStart();
    // Setup window perspective.
    Graphics::Clear();
    m_camera.ApplyMatrix();

    OnPreRender();

    Filters::reverse_iterator currentBottomFilterList =
        m_bottomFilters.rbegin();
    Filters ::reverse_iterator currentTopFilterList = m_topFilters.rbegin();

    for (Layers::reverse_iterator layer = m_layers.rbegin();
         layer != m_layers.rend(); layer++) {
        int layerNum = layer->first;
        if (layerNum <= m_camera.backLayer() &&
            layerNum >= m_camera.frontLayer()) {

            /* While we have a filter that should end on a layer higher
             * than the layer of entities we just finished rendering, alert the
             * filter and increment the filter iterator. */
            while (currentTopFilterList != m_topFilters.rend() &&
                currentTopFilterList->first >= layerNum) {
                FOR_ (list<Filter*>::iterator, i,
                      currentTopFilterList->second) {
                    if ((*i)->enabled())
                        (*i)->OnTopLayer();
                }
                currentTopFilterList++;
            }

            /* While we have a filter that should start on a layer lower
             * than the layer of entities we're about to render, alert the
             * filter and increment the filter iterator. */
            while (currentBottomFilterList != m_bottomFilters.rend() &&
                currentBottomFilterList->first >= layerNum) {
                FOR_ (list<Filter*>::iterator, i,
                      currentBottomFilterList->second) {
                    if ((*i)->enabled())
                        (*i)->OnBottomLayer();
                }
                currentBottomFilterList++;
            }

            list<Entity*>& entityList = layer->second;
            FOR_ (list<Entity*>::iterator, i, entityList) {
                if (!(*i)->visible())
                    continue;

                if (m_hook != NULL)
                    m_hook->OnEntityRenderStart(*i);
                (*i)->Render();
                if (m_hook != NULL)
                    m_hook->OnEntityRenderEnd(*i);
                #if DEBUG
                    Graphics::CheckErrors();
                    Graphics::CheckState();
                #endif
            }
        }
    }

    // Alert remaining top filters.
    for (; currentTopFilterList != m_topFilters.rend();
         currentTopFilterList++) {
        int layerNum = currentTopFilterList->first;
        if (layerNum <= m_camera.backLayer() &&
            layerNum >= m_camera.frontLayer()) {
            FOR_ (list<Filter*>::iterator, i,
                  currentTopFilterList->second) {
                if ((*i)->enabled())
                    (*i)->OnTopLayer();
            }
        }
    }

    if (G_PhysicsManager != NULL)
        G_PhysicsManager->Render();

    OnPostRender();

#if DEBUG
    Graphics::RenderAxes();
    Graphics::CheckErrors();
    Graphics::CheckState();
#endif
    Graphics::ShowBuffer();

    if (m_hook != NULL)
        m_hook->OnRenderEnd();
}

void Scene::AddFilter (Filter* filter) {
    m_filtersToAdd.insert(filter);
}

void Scene::ResolveFilterChanges () {
    // This works only if OnAdded doesn't make more filters.
    FOR_ (set<Filter*>::iterator, i, m_filtersToAdd) {
        Filter* filter = *i;
        m_bottomFilters[filter->bottomLayer()].push_back(filter);
        m_topFilters[filter->topLayer()].push_back(filter);
        filter->OnAdded();
    }
    m_filtersToAdd.clear();

    FOR_ (set<Filter*>::iterator, i, m_filtersToRemove) {
        Filter* filter = *i;
        list<Filter*>& filterList = m_bottomFilters[filter->bottomLayer()];
        list<Filter*>::iterator filterIter = find(filterList.begin(),
                                                  filterList.end(),
                                                  filter);
        if (filterIter == filterList.end()) {
            LOG(WARNING) << "Failed to find filter in list for removal.";
            return;
        }
        filterList.erase(filterIter);

        filterList = m_topFilters[filter->topLayer()];
        filterIter = find(filterList.begin(), filterList.end(), filter);
        if (filterIter == filterList.end()) {
            LOG(WARNING) << "Failed to find filter in list for removal.";
            return;
        }
        filterList.erase(filterIter);

        delete filter;
    }
}

void Scene::RemoveFilter (Filter* filter) {
    m_filtersToRemove.insert(filter);
}

class ListEntitiesQueryCallback : public b2QueryCallback {
public:
    ListEntitiesQueryCallback (Vector2 upperLeft, Vector2 lowerRight)
        : upperLeft(upperLeft),
          lowerRight(lowerRight) { }

    bool ReportFixture (b2Fixture* fixture) {
        b2Body* body = fixture->GetBody();
        Entity* e = (Entity*)body->GetUserData();
        bool isInBox = false;
        for (float x = upperLeft.x; x < lowerRight.x; x += 0.3) {
            for (float y = upperLeft.y; y < lowerRight.y; y += 0.3) {
                if (fixture->GetShape()->TestPoint(body->GetTransform(),
                                                   b2Vec2(x, y))) {
                    isInBox = true;
                    break;
                }
            }
        }

        if (isInBox)
            m_entityList.push_back(e);
        return true;
    }
    vector<Entity*> m_entityList;
    Vector2 upperLeft;
    Vector2 lowerRight;
};

void Scene::GetEntitiesAtPoint (Vector2 point, vector<Entity*>* entities) {
    GetEntitiesInArea(point - Vector2(0.1f, 0.1f), point + Vector2(0.1f, 0.1f),
                      entities);
}

void Scene::GetEntitiesInArea (Vector2 point1, Vector2 point2,
                               vector<Entity*>* entities) {
    float ux = MIN(point1.x, point2.x);
    float uy = MIN(point1.y, point2.y);
    float lx = MAX(point1.x, point2.x);
    float ly = MAX(point1.y, point2.y);
    point1.Set(ux, uy);
    point2.Set(lx, ly);

    ListEntitiesQueryCallback callback(point1, point2);
    b2AABB aabb;
    aabb.lowerBound.Set(point1.x, point1.y);
    aabb.upperBound.Set(point2.x, point2.y);
    G_PhysicsManager->world()->QueryAABB(&callback, aabb);

    entities->resize(callback.m_entityList.size());
    copy(callback.m_entityList.begin(), callback.m_entityList.end(),
         entities->begin());
}

void Scene::OnEntityLayerChange (Entity* entity, int oldLayer, int newLayer) {
    Layers::iterator found;
    found = m_layers.find(oldLayer);
    bool wasFound = false;
    if (found != m_layers.end()) {
        list<Entity*>& entityList = found->second;
        list<Entity*>::iterator entityI = find(entityList.begin(),
                                               entityList.end(),
                                               entity);
        if (entityI != entityList.end()) {
            entityList.erase(entityI);
            wasFound = true;
        }
    }

    if (!wasFound) {
        LOG(ERROR) << "Could not find entity " << *entity << " in old layer "
                   << oldLayer << ".";
    }

    found = m_layers.find(newLayer);
    if (found != m_layers.end()) {
        found->second.push_back(entity);
    } else {
        list<Entity*> entityList;
        entityList.push_back(entity);
        m_layers.insert(make_pair(newLayer, entityList));
    }
}

void Scene::GetLayerEntityCounts (map<int, int>* mapOut) {
    FOR_(Layers::iterator, layer, m_layers) {
        (*mapOut)[layer->first] = layer->second.size();
    }
}

void Scene::GetEntities (vector<Entity*>* entities) const {
    FOR_(Layers::const_iterator, layer, m_layers) {
        FOR_(list<Entity*>::const_iterator, i, layer->second) {
            entities->push_back(*i);
        }
    }
}

void Scene::SetHook (SceneHook* hook) {
    m_hook = hook;
    m_hook->m_scene = this;
}

Entity* Scene::GetFirstTagged (const string& tag) {
    list<Entity*>& list = m_entityTags[tag];
    if (list.size() == 0)
        return NULL;
    return list.front();
}

void Scene::GetAllTagged (const string& tag, vector<Entity*>* entities) {
    list<Entity*>& list = m_entityTags[tag];
    entities->resize(list.size());
    copy(list.begin(), list.end(), entities->begin());
}

void Scene::OnEntityTagAdd (Entity* entity, const string& tag) {
    m_entityTags[tag].push_back(entity);
}

void Scene::OnEntityTagRemove (Entity* entity, const string& tag) {
    m_entityTags[tag].remove(entity);
}

Filter* Scene::GetFilter (const string& filterName) {
    FOR_ (Filters::iterator, i, m_bottomFilters) {
        FOR_(list<Filter*>::iterator, filterIter, i->second)
            if ((*filterIter)->name() == filterName)
                return *filterIter;
    }
    return NULL;
}

}
