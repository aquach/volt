#include "Scene.h"
#include <algorithm>
#include "Entity.h"
#include "Graphics/Filter.h"
#include "Graphics/Graphics.h"

namespace Volt {

Scene::Scene ()
    : m_isPaused(false) {
}

Scene::~Scene () {
    RemoveAll();
    ResolveEntityChanges();
}

void Scene::RemoveAll () {
    for (Layers::iterator layer = m_layers.begin();
         layer != m_layers.end();
         layer++) {
        list<Entity*>& entityList = layer->second;
        m_entitiesToRemove.insert(m_entitiesToRemove.end(),
                                  entityList.begin(), entityList.end());
    }
}

void Scene::Update () {
    if (!m_isPaused) {
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
    m_entitiesToAdd.push_back(entity);
}

void Scene::Remove (Entity* entity) {
    m_entitiesToRemove.push_back(entity);
}

void Scene::ResolveEntityChanges () {
    for (list<Entity*>::iterator i = m_entitiesToRemove.begin();
         i != m_entitiesToRemove.end(); i++) {
        Entity* entity = *i;

        Layers::iterator found = m_layers.find(entity->layer());
        if (found != m_layers.end()) {
            list<Entity*>& entityList = found->second;
            list<Entity*>::iterator entityI = find(entityList.begin(),
                                                   entityList.end(),
                                                   entity);
            if (entityI != entityList.end()) {
                entityList.erase(entityI);
            } else {
                LOG(WARNING) << "Removing entity: Could not find entity "
                             << "in layers map." << endl;
            }
        } else {
            LOG(WARNING) << "Removing entity: Could not find entity "
                         << "in layers map." << endl;
        }

        entity->m_scene = NULL;
        delete entity;
    }
    m_entitiesToRemove.clear();

    for (list<Entity*>::iterator i = m_entitiesToAdd.begin();
         i != m_entitiesToAdd.end(); i++) {
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
    }

    m_entitiesToAdd.clear();
}

void Scene::RenderFilter (Filter* filter) {
    // TODO.
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
            list<Entity*>& entityList = layer->second;
            for (list<Entity*>::iterator i = entityList.begin();
                 i != entityList.end();
                 i++) {
                (*i)->Render();
            }

            if (currentFilter != m_filters.end()) {
                if ((*currentFilter)->layer() <= layerNum) {
                    RenderFilter(*currentFilter);
                    currentFilter++;

                    // Restore normal projection matrix.
                    m_camera.ApplyMatrix();
                }
            }
        }
    }

    // Render remaining filters on top.
    for (; currentFilter != m_filters.end(); currentFilter++) {
        int layerNum = (*currentFilter)->layer();
        if (layerNum <= m_camera.backLayer() &&
            layerNum >= m_camera.frontLayer()) {
            RenderFilter(*currentFilter);
        }
    }

    Graphics::ShowBuffer();
}

void Scene::AddFilter (Filter* filter) {
    list<Filter*>::iterator i = m_filters.begin();
    while (i != m_filters.end() && (*i)->layer() < filter->layer())
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
