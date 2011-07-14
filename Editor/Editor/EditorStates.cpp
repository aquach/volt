#include "Editor/Editor/Editor.h"
#include <QtGui>
#include "Game/Editor/SelectionManager.h"
#include "Game/Entities/Game/Triangle.h"
#include "Editor/Editor/EditorScene.h"
#include "Editor/Editor/GLWidget.h"
#include "Editor/Editor/PropertyModel.h"

const float ANGLE_SNAP = 5.0f;

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

void Editor::SelectState::OnViewportMouseRelease (QMouseEvent* event) {
    m_dragging = false;
}

void Editor::SelectState::OnViewportMousePress (QMouseEvent* event) {
    event->accept();
    Vector2 screenPos(event->pos().x(), event->pos().y());
    Entity* selectedEntity = m_e->GetTopEntityAtPoint(screenPos);

    if (selectedEntity == NULL) {
        if (!m_e->m_appendMode && !m_e->m_removeMode) {
            m_dragging = true;
            m_startPos = m_e->m_scene->camera()->ScreenToWorld(screenPos);
            m_currentPos = m_startPos;
            G_SelectionManager->DeselectAll();
        }
        return;
    }

    if (m_e->m_appendMode) {
        G_SelectionManager->SelectEntity(selectedEntity);
    } else if (m_e->m_removeMode) {
        G_SelectionManager->DeselectEntity(selectedEntity);
    } else {
        G_SelectionManager->DeselectAll();
        G_SelectionManager->SelectEntity(selectedEntity);
        m_e->m_propertyModel->SetEntity(selectedEntity);
        m_e->m_properties->resizeColumnsToContents();
        m_e->OnModified();
    }
}

void Editor::SelectState::OnViewportMouseMove (QMouseEvent* event) {
    event->accept();
    Vector2 screenPos(event->pos().x(), event->pos().y());
    if (m_dragging) {
        Vector2 pos = m_e->m_scene->camera()->ScreenToWorld(screenPos);
        m_currentPos = pos;
        vector<Volt::Entity*> entities;
        m_e->m_scene->GetEntitiesInArea(pos, m_startPos, &entities);
        for (uint i = 0; i < entities.size(); i++) {
            if (Entity* e = dynamic_cast<Entity*>(entities[i])) {
                G_SelectionManager->SelectEntity(e);
            }
        }
    } else {
        Entity* selectedEntity = m_e->GetTopEntityAtPoint(screenPos);

        if (selectedEntity != NULL) {
            m_e->m_viewport->setCursor(Qt::CrossCursor);
        } else {
            m_e->m_viewport->setCursor(Qt::ArrowCursor);
        }
    }
}

void Editor::SelectState::Render () {
    if (m_dragging) {
        glLineWidth(1.0f);
        Graphics::SetColor(Volt::Color::white);
        Graphics::RenderLineRect(m_startPos, m_currentPos);
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
    event->accept();
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
    event->accept();
    int selectedVertex;
    Triangle* selectedTriangle = m_e->GetTopVertexAtPoint(
        Vector2(event->pos().x(), event->pos().y()), &selectedVertex);

    if (selectedTriangle != NULL) {
        m_e->m_viewport->setCursor(Qt::CrossCursor);
    } else {
        m_e->m_viewport->setCursor(Qt::ArrowCursor);
    }
}

void Editor::MoveState::OnEnter () {
    m_e->m_viewport->setCursor(Qt::ArrowCursor);
}

void Editor::MoveState::OnExit () {
}

void Editor::MoveState::OnViewportMousePress (QMouseEvent* event) {
    event->accept();
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);
    if (selectedEntities.size() <= 1) {
        // Allow selection of individuals.
        Entity* selectedEntity = m_e->GetTopEntityAtPoint(
            Vector2(event->pos().x(), event->pos().y()));

        G_SelectionManager->DeselectAll();
        if (selectedEntity != NULL) {
            selectedEntities.push_back(selectedEntity);
            G_SelectionManager->SelectEntity(selectedEntity);
        } else {
            return;
        }
    }

    m_dragging = true;
    m_startPoint = Vector2(event->pos().x(), event->pos().y());
    m_startPoint = m_e->m_scene->camera()->ScreenToWorld(m_startPoint);
    G_SelectionManager->GetSelectedEntities(&m_selectedEntities);
    FOR_(vector<Entity*>::iterator, i, m_selectedEntities) {
        Entity* entity = *i;
        m_startPositions.push_back(entity->position());
    }
}

void Editor::MoveState::OnViewportMouseRelease (QMouseEvent* event) {
    m_dragging = false;
    event->accept();
    m_selectedEntities.clear();
    m_startPositions.clear();
}

void Editor::MoveState::OnViewportMouseMove (QMouseEvent* event) {
    if (m_dragging) {
        m_e->OnModified();
        Vector2 pos(event->pos().x(), event->pos().y());
        Vector2 worldPos = m_e->m_scene->camera()->ScreenToWorld(pos);
        Vector2 dp = worldPos - m_startPoint;

        for (uint i = 0; i < m_selectedEntities.size(); i++) {
            Entity* entity = m_selectedEntities[i];
            Vector2 newPos = m_startPositions[i] + dp;
            if (m_e->m_snapOn)
                newPos = m_e->SnapToGrid(newPos);
            entity->SetPosition(newPos);
        }
        event->accept();
    } else {
        event->ignore();
    }
}

void Editor::RotateState::OnEnter () {
    m_e->m_viewport->setCursor(Qt::ArrowCursor);
}

void Editor::RotateState::OnExit () {
}

void Editor::RotateState::OnViewportMousePress (QMouseEvent* event) {
    event->accept();
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);
    if (selectedEntities.size() <= 1) {
        // Allow selection of individuals.
        Entity* selectedEntity = m_e->GetTopEntityAtPoint(
            Vector2(event->pos().x(), event->pos().y()));

        G_SelectionManager->DeselectAll();
        if (selectedEntity != NULL) {
            selectedEntities.push_back(selectedEntity);
            G_SelectionManager->SelectEntity(selectedEntity);
        } else {
            return;
        }
    }

    m_startPoint = Vector2(event->pos().x(), event->pos().y());
    m_startPoint = m_e->m_scene->camera()->ScreenToWorld(m_startPoint);
    G_SelectionManager->GetSelectedEntities(&m_selectedEntities);
    FOR_(vector<Entity*>::iterator, i, m_selectedEntities) {
        Entity* entity = *i;
        m_startRotations.push_back(entity->rotation());
    }
    m_dragging = true;
}

void Editor::RotateState::OnViewportMouseRelease (QMouseEvent* event) {
    m_selectedEntities.clear();
    m_startRotations.clear();
    m_dragging = false;
    event->accept();
}

void Editor::RotateState::OnViewportMouseMove (QMouseEvent* event) {
    if (m_dragging) {
        m_e->OnModified();
        Vector2 pos(event->pos().x(), event->pos().y());
        Vector2 worldPos = m_e->m_scene->camera()->ScreenToWorld(pos);
        Vector2 pivot = GetWorldPivotPoint();
        Vector2 dStart = m_startPoint - pivot;
        Vector2 dPos = worldPos - pivot;

        float angle = dStart.AngleTo(dPos);

        for (uint i = 0; i < m_selectedEntities.size(); i++) {
            Entity* entity = m_selectedEntities[i];
            float newRot = m_startRotations[i] + angle;
            if (m_e->m_snapOn)
                newRot = Volt::RoundToNearest(newRot, ANGLE_SNAP);
            entity->SetRotation(newRot);
        }
        event->accept();
    } else {
        event->ignore();
    }
}

Vector2 Editor::RotateState::GetWorldPivotPoint () {
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);

    if (selectedEntities.size() == 0)
        return Vector2();

    Vector2 sum;
    FOR_(vector<Entity*>::iterator, i, selectedEntities) {
        Entity* entity = *i;
        sum += entity->position();
    }
    return sum / selectedEntities.size();
}

void Editor::RotateState::Render () {
    glPushMatrix();
    glLineWidth(1.0f);
    Graphics::SetColor(Volt::Color::RGB(200, 200, 200));
    Graphics::Translate(GetWorldPivotPoint());
    Graphics::RenderLine(Vector2(-1, 0), Vector2(1, 0));
    Graphics::RenderLine(Vector2(0, -1), Vector2(0, 1));
    Graphics::SetColor(Volt::Color::RGB(200, 100, 0));
    Graphics::RenderLineCircle(4.0f);
    glPopMatrix();
}

void Editor::ScaleState::OnEnter () {
    m_e->m_viewport->setCursor(Qt::ArrowCursor);
}

void Editor::ScaleState::OnExit () {
}

void Editor::ScaleState::OnViewportMousePress (QMouseEvent* event) {
    event->accept();
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);
    if (selectedEntities.size() <= 1) {
        // Allow selection of individuals.
        Entity* selectedEntity = m_e->GetTopEntityAtPoint(
            Vector2(event->pos().x(), event->pos().y()));

        G_SelectionManager->DeselectAll();
        if (selectedEntity != NULL) {
            selectedEntities.push_back(selectedEntity);
            G_SelectionManager->SelectEntity(selectedEntity);
        } else {
            return;
        }
    }

    m_dragging = true;
    m_lastPoint = event->pos();
}

void Editor::ScaleState::OnViewportMouseRelease (QMouseEvent* event) {
    m_dragging = false;
    event->accept();
}

void Editor::ScaleState::OnViewportMouseMove (QMouseEvent* event) {
    if (m_dragging) {
        m_e->OnModified();
        Vector2 lastPoint(m_lastPoint.x(), m_lastPoint.y());
        Vector2 pos(event->pos().x(), event->pos().y());
        m_lastPoint = event->pos();

        Vector2 pivot = GetWorldPivotPoint();
        Vector2 worldPos = m_e->m_scene->camera()->ScreenToWorld(pos);
        Vector2 lastPointPos = m_e->m_scene->camera()->ScreenToWorld(lastPoint);
        Vector2 dLast = lastPointPos - pivot;
        Vector2 dPos = worldPos - pivot;

        //float ratio = dPos.LengthSquared() / dLast.LengthSquared();

        vector<Entity*> selectedEntities;
        G_SelectionManager->GetSelectedEntities(&selectedEntities);
        FOR_(vector<Entity*>::iterator, i, selectedEntities) {
            //Entity* entity = *i;
            //entity->SetScale(entity->scale() * ratio);
        }
        event->accept();
    } else {
        event->ignore();
    }
}

Vector2 Editor::ScaleState::GetWorldPivotPoint () {
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);

    if (selectedEntities.size() == 0)
        return Vector2();

    Vector2 sum;
    FOR_(vector<Entity*>::iterator, i, selectedEntities) {
        Entity* entity = *i;
        sum += entity->position();
    }
    return sum / selectedEntities.size();
}

void Editor::ScaleState::Render () {
    glPushMatrix();
    glLineWidth(1.0f);
    Graphics::SetColor(Volt::Color::RGB(200, 200, 200));
    Graphics::Translate(GetWorldPivotPoint());
    Graphics::RenderLine(Vector2(-1, 0), Vector2(1, 0));
    Graphics::RenderLine(Vector2(0, -1), Vector2(0, 1));
    Graphics::SetColor(Volt::Color::RGB(100, 200, 0));
    Graphics::RenderLineCircle(4.0f, 4);
    glPopMatrix();
}

