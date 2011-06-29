#include "Scene.h"
#include <algorithm>
#include "Entity.h"
#include "Graphics/Filter.h"
#include "Graphics/Graphics.h"
#include "Graphics/OpenGL.h"
#include "PhysicsManager.h"

namespace Volt {

Scene::Scene ()
    : m_isPaused(false) {
}

Scene::~Scene () {
    RemoveAll();
    ResolveEntityChanges();

    for (list<Filter*>::iterator i = m_filters.begin(); i != m_filters.end();
         i++) {
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
        for (Layers::iterator layer = m_layers.begin();
             layer != m_layers.end();
             layer++) {
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
            for (list<Entity*>::iterator i = entityList.begin();
                 i != entityList.end();
                 i++) {
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

    G_PhysicsManager->Render();

    glLineWidth(4.0);
    Graphics::SetColor(Color::red);
    Graphics::RenderLine(Vector2(0, 0), Vector2(1, 0));
    Graphics::SetColor(Color::green);
    Graphics::RenderLine(Vector2(0, 0), Vector2(0, 1));

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

}
