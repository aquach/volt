#include "Scene.h"
#include "Entity.h"

namespace Volt {

Scene::Scene ()
    : m_isPaused(false) {
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

void Scene::ResolveEntityChanges () {
    for (list<Entity*>::iterator i = m_entitiesToAdd.begin();
         i != m_entitiesToAdd.end(); i++) {
        // TODO
    }
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
