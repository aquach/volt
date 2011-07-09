#include "Volt/Game/Scene.h"
#include <algorithm>
#include "Volt/Game/Entity.h"
#include "Volt/Graphics/Filter.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"
#include "Volt/Game/PhysicsManager.h"

namespace Volt {

Scene::Scene ()
    : m_isPaused(false),
      m_isEditor(false) {
}

Scene::~Scene () {
    RemoveAll();
    ResolveEntityChanges();

    FOR_ (list<Filter*>::iterator, i, m_filters) {
        delete *i;
    }
    m_filters.clear();
}

void Scene::RemoveAll () {
    for (Layers::iterator layer = m_layers.begin();
         layer != m_layers.end();
         layer++) {
        list<Entity*>& entityList = layer->second;
        m_entitiesToRemove.insert(entityList.begin(), entityList.end());
    }
}

void Scene::Update () {
    if (!m_isPaused) {
        m_camera.Update();
        FOR_ (Layers::iterator, layer, m_layers) {
            list<Entity*>& entityList = layer->second;
            for (list<Entity*>::iterator i = entityList.begin();
                 i != entityList.end();
                 i++) {
                (*i)->Update();
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
                (*currentFilter)->Render();
                currentFilter++;
            }

            list<Entity*>& entityList = layer->second;
            FOR_ (list<Entity*>::iterator, i, entityList) {
                (*i)->Render();
            }
        }
    }

    // Render remaining filters on top.
    for (; currentFilter != m_filters.end(); currentFilter++) {
        int layerNum = (*currentFilter)->layer();
        if (layerNum <= m_camera.backLayer() &&
            layerNum >= m_camera.frontLayer()) {
            (*currentFilter)->Render();
        }
    }

    if (G_PhysicsManager != NULL)
        G_PhysicsManager->Render();

    OnPostRender();

    Graphics::RenderAxes();
    Graphics::CheckErrors();
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
    ListEntitiesQueryCallback (Vector2 point) : point(point) { }
    bool ReportFixture (b2Fixture* fixture) {
        b2Body* body = fixture->GetBody();
        Entity* e = (Entity*)body->GetUserData();
        if (fixture->GetShape()->TestPoint(body->GetTransform(), point.ToB2()))
            m_entityList.push_back(e);
        return true;
    }
    vector<Entity*> m_entityList;
    Vector2 point;
};

void Scene::GetEntitiesAtPoint (Vector2 point, vector<Entity*>* entities) {
    ListEntitiesQueryCallback callback(point);
    b2AABB aabb;
    aabb.lowerBound.Set(point.x - 0.1, point.y - 0.1);
    aabb.upperBound.Set(point.x + 0.1, point.y + 0.1);
    G_PhysicsManager->world()->QueryAABB(&callback, aabb);

    entities->resize(callback.m_entityList.size());
    copy(callback.m_entityList.begin(), callback.m_entityList.end(),
         entities->begin());
}

}
