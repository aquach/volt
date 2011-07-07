#include "Editor/Editor/Editor.h"
#include <QtGui>
#include "Game/Editor/SelectionManager.h"
#include "Game/Entities/Game/Triangle.h"
#include "Editor/Editor/EditorScene.h"
#include "Editor/Editor/GLWidget.h"

void Editor::PanState::OnEnter () {
    m_e->m_viewport->setCursor(Qt::OpenHandCursor);
}

void Editor::PanState::OnExit () {
    m_e->m_viewport->setCursor(Qt::ArrowCursor);
}

void Editor::PanState::OnViewportMousePress (QMouseEvent* event) {
    m_e->m_viewport->setCursor(Qt::ClosedHandCursor);
    m_dragging = true;
    m_lastPoint = event->pos();
    event->accept();
}

void Editor::PanState::OnViewportMouseRelease (QMouseEvent* event) {
    m_e->m_viewport->setCursor(Qt::OpenHandCursor);
    m_dragging = false;
    event->accept();
}

void Editor::PanState::OnViewportMouseMove (QMouseEvent* event) {
    if (m_dragging) {
        Vector2 lastPoint(m_lastPoint.x(), m_lastPoint.y());
        Vector2 pos(event->pos().x(), event->pos().y());
        m_lastPoint = event->pos();

        Vector2 worldPos = m_e->m_scene->camera()->ScreenToWorld(pos);
        Vector2 lastPointPos = m_e->m_scene->camera()->ScreenToWorld(lastPoint);
        Vector2 dp = worldPos - lastPointPos;
        m_e->m_scene->camera()->transform.position -= dp;
        event->accept();
    } else {
        event->ignore();
    }
}

void Editor::SelectState::OnEnter () {
    m_e->m_viewport->setCursor(Qt::ArrowCursor);
}

void Editor::SelectState::OnExit () {
}

void Editor::SelectState::OnViewportMousePress (QMouseEvent* event) {
    Volt::Entity* selectedEntity = m_e->GetTopEntityAtPoint(
        Vector2(event->pos().x(), event->pos().y()));

    if (selectedEntity == NULL) {
        if (!m_e->m_appendMode && !m_e->m_removeMode)
            G_SelectionManager->DeselectAll();
        return;
    }

    if (m_e->m_appendMode) {
        G_SelectionManager->SelectEntity(selectedEntity);
    } else if (m_e->m_removeMode) {
        G_SelectionManager->DeselectEntity(selectedEntity);
    } else {
        G_SelectionManager->DeselectAll();
        G_SelectionManager->SelectEntity(selectedEntity);
    }
}

void Editor::SelectState::OnViewportMouseMove (QMouseEvent* event) {
    Vector2 pos = m_e->m_scene->camera()->ScreenToWorld(
                    Vector2(event->pos().x(), event->pos().y()));
    vector<Volt::Entity*> entities;
    m_e->m_scene->GetEntitiesAtPoint(pos, &entities);
    if (entities.size() > 0) {
        m_e->m_viewport->setCursor(Qt::CrossCursor);
    } else {
        m_e->m_viewport->setCursor(Qt::ArrowCursor);
    }
}

void Editor::SelectVerticesState::OnEnter () {
    m_e->m_viewport->setCursor(Qt::ArrowCursor);
    m_e->m_selectionManager->SetShowVertices(true);
}

void Editor::SelectVerticesState::OnExit () {
    m_e->m_selectionManager->SetShowVertices(false);
}

void Editor::SelectVerticesState::OnViewportMousePress (QMouseEvent* event) {
    int selectedVertex;
    Triangle* selectedTriangle = m_e->GetTopVertexAtPoint(
        Vector2(event->pos().x(), event->pos().y()), &selectedVertex);
        
    if (selectedTriangle == NULL) {
        if (!m_e->m_appendMode && !m_e->m_removeMode)
            G_SelectionManager->DeselectAll();
        return;
    }
    
    if (m_e->m_appendMode) {
        G_SelectionManager->SelectVertex(selectedTriangle,
                                         selectedVertex);
    } else if (m_e->m_removeMode) {
        G_SelectionManager->DeselectVertex(selectedTriangle,
                               selectedVertex);
    } else {
        G_SelectionManager->DeselectAll();
        G_SelectionManager->SelectVertex(selectedTriangle, selectedVertex);
    }
}

void Editor::SelectVerticesState::OnViewportMouseMove (QMouseEvent* event) {
}

void Editor::MoveState::OnEnter () {
    m_e->m_viewport->setCursor(Qt::OpenHandCursor);
}

void Editor::MoveState::OnExit () {
    m_e->m_viewport->setCursor(Qt::ArrowCursor);
}

void Editor::MoveState::OnViewportMousePress (QMouseEvent* event) {
    /*
    m_e->m_viewport->setCursor(Qt::ClosedHandCursor);
    m_dragging = true;
    m_lastPoint = event->pos();
    event->accept();
    */
}

void Editor::MoveState::OnViewportMouseRelease (QMouseEvent* event) {
    /*
    m_e->m_viewport->setCursor(Qt::OpenHandCursor);
    m_dragging = false;
    event->accept();
    */
}

void Editor::MoveState::OnViewportMouseMove (QMouseEvent* event) {
    /*
    if (m_dragging) {
        Vector2 lastPoint(m_lastPoint.x(), m_lastPoint.y());
        Vector2 pos(event->pos().x(), event->pos().y());
        m_lastPoint = event->pos();

        Vector2 worldPos = m_e->m_scene->camera()->ScreenToWorld(pos);
        Vector2 lastPointPos = m_e->m_scene->camera()->ScreenToWorld(lastPoint);
        Vector2 dp = worldPos - lastPointPos;
        m_e->m_scene->camera()->transform.position -= dp;
        event->accept();
    } else {
        event->ignore();
    }
    */
}
