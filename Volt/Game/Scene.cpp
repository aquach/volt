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

    FOR_ (list<Filter*>::iterator, i, m_filters) {
        delete *i;
    }
    m_filters.clear();
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
}

void Scene::Add (Entity* entity, int layer) {
    entity->m_layer = layer;
    m_entitiesToAdd.insert(entity);
}

void Scene::Remove (Entity* entity) {
    m_entitiesToRemove.insert(entity);
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
                entity->OnRemoved();
                entity->m_scene = NULL;
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

        Layers::iterator found = m_layers.find(entity->layer());
        if (found != m_layers.end()) {
            found->second.push_back(entity);
        } else {
            list<Entity*> entityList;
            entityList.push_back(entity);
            m_layers.insert(make_pair(entity->layer(), entityList));
        }
        entity->m_scene = this;
        entity->OnAdded();
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

    list<Filter*>::iterator currentFilter = m_filters.begin();

    for (Layers::reverse_iterator layer = m_layers.rbegin();
         layer != m_layers.rend(); layer++) {
        int layerNum = layer->first;
        if (layerNum <= m_camera.backLayer() &&
            layerNum >= m_camera.frontLayer()) {

            /* While we have a filter that should go on a layer lower
             * than the layer of entities we're about to render, render
             * the filter and increment the filter iterator. */
            while (currentFilter != m_filters.end() &&
                   (*currentFilter)->layer() >= layerNum) {
                if (m_hook != NULL)
                    m_hook->OnFilterRenderStart(*currentFilter);
                (*currentFilter)->Render();
                if (m_hook != NULL)
                    m_hook->OnFilterRenderEnd(*currentFilter);
                currentFilter++;
            }

            list<Entity*>& entityList = layer->second;
            FOR_ (list<Entity*>::iterator, i, entityList) {
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

    // Render remaining filters on top.
    for (; currentFilter != m_filters.end(); currentFilter++) {
        int layerNum = (*currentFilter)->layer();
        if (layerNum <= m_camera.backLayer() &&
            layerNum >= m_camera.frontLayer()) {
            if (m_hook != NULL)
                m_hook->OnFilterRenderStart(*currentFilter);
            (*currentFilter)->Render();
            if (m_hook != NULL)
                m_hook->OnFilterRenderEnd(*currentFilter);
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
}

void Scene::AddFilter (Filter* filter, int layer) {
    filter->m_layer = layer;

    list<Filter*>::iterator i = m_filters.begin();
    while (i != m_filters.end() && (*i)->layer() > filter->layer())
        i++;
    m_filters.insert(i, filter);
}

void Scene::RemoveFilter (Filter* filter) {
    for (list<Filter*>::iterator i = m_filters.begin(); i != m_filters.end();) {
        if (*i == filter) {
            i = m_filters.erase(i);
            break;
        } else {
            i++;
        }
    }
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
    GetEntitiesInArea(point - Vector2(0.1, 0.1), point + Vector2(0.1, 0.1),
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

}
