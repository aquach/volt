#pragma once

#include "Editor/Core/Core.h"
#include <set>

class Entity;
class Triangle;

/**
 *  Manages selecting vertices of triangles or entities. Any number of either
 *  can be selected, but not some of both types.
 */
class SelectionManager {
public:
    class SelectionListener {
    public:
        virtual ~SelectionListener () { }
        virtual void OnEntitySelected (Entity* e) = 0;
        virtual void OnEntityDeselected (Entity* e) = 0;
        virtual void OnDeselectAll () = 0;
    };

    SelectionManager () : m_showVertices(false) { }
    ~SelectionManager () { }

    static void Register (SelectionManager* manager) { instance = manager; }
    static SelectionManager* Instance () { return instance; }

    bool IsVertexSelected (Triangle* triangle, int vertex) const {
        Vertex v = { triangle, vertex };
        return m_selectedVertices.count(v) > 0;
    }
    bool IsEntitySelected (Entity* entity) const {
        return m_selectedEntities.count(entity) > 0;
    }

    bool showVertices () const { return m_showVertices; }
    void SetShowVertices (bool show) { m_showVertices = show; }

    void GetSelectedEntities (vector<Entity*>* entities) const {
        entities->resize(m_selectedEntities.size());
        copy(m_selectedEntities.begin(), m_selectedEntities.end(), entities->begin());
    }
    void GetSelectedVertices (vector<Triangle*>* triangles, vector<int>* vertices) const {
        FOR_(set<Vertex>::const_iterator, i, m_selectedVertices) {
            triangles->push_back(i->triangle);
            vertices->push_back(i->vertex);
        }
    }

    void SelectVertex (Triangle* triangle, int vertex) {
        Vertex v = { triangle, vertex };
        m_selectedVertices.insert(v);
        m_selectedEntities.clear();
    }
    void SelectEntity (Entity* entity) {
        m_selectedEntities.insert(entity);
        m_selectedVertices.clear();
        FOR_(set<SelectionListener*>::iterator, i, m_selectionListeners)
            (*i)->OnEntitySelected(entity);
    }
    void DeselectVertex (Triangle* triangle, int vertex) {
        Vertex v = { triangle, vertex };
        m_selectedVertices.erase(v);
    }
    void DeselectEntity (Entity* entity) {
        m_selectedEntities.erase(entity);
        FOR_(set<SelectionListener*>::iterator, i, m_selectionListeners)
            (*i)->OnEntityDeselected(entity);
    }
    void DeselectAll () {
        m_selectedVertices.clear();
        m_selectedEntities.clear();
        FOR_(set<SelectionListener*>::iterator, i, m_selectionListeners)
            (*i)->OnDeselectAll();
    }

    int numSelectedEntities () const { return m_selectedEntities.size(); }
    int numSelectedVertices () const { return m_selectedVertices.size(); }

    void Render ();

    void AddSelectionListener (SelectionListener* listener) {
        m_selectionListeners.insert(listener);
    }
    void RemoveSelectionListener (SelectionListener* listener) {
        m_selectionListeners.erase(listener);
    }

private:
    struct Vertex {
        Triangle* triangle;
        int vertex;

        bool operator< (const Vertex& other) const {
            if (triangle < other.triangle)
              return true;
            if (triangle > other.triangle)
              return false;
            return vertex < other.vertex;
        }
    };
    set<Vertex> m_selectedVertices;
    set<Entity*> m_selectedEntities;
    set<SelectionListener*> m_selectionListeners;
    bool m_showVertices;

    static SelectionManager* instance;

    DISALLOW_COPY_AND_ASSIGN(SelectionManager);
};

#define G_SelectionManager SelectionManager::Instance()
