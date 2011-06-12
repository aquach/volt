#include "Scene.h"
#include <algorithm>
#include "Entity.h"

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
        copy(entityList.begin(), entityList.end(), m_entitiesToRemove.begin());
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

void Scene::Add (Entity* entity) {
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

void Scene::Render () {
    // Setup window perspective stuff.

    for (Layers::iterator layer = m_layers.begin(); layer != m_layers.end();
         layer++) {
        list<Entity*>& entityList = layer->second;
        for (list<Entity*>::iterator i = entityList.begin();
             i != entityList.end();
             i++) {
            (*i)->Render();
        }
    }
}

}
