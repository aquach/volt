#pragma once

#include "Editor/Core/Core.h"
#include <QMainWindow>
#include "Volt/Game/FSM.h"

namespace Volt {
    class AssetManager;
    class DataSource;
    class Graphics;
    class PhysicsManager;
}

extern const char* EDITOR_TITLE;

class EditorScene;
class Entity;
class GLWidget;
class PropertyModel;
class QLabel;
class QSettings;
class SelectionManager;
class Triangle;

class Editor : public QMainWindow {
    Q_OBJECT;

public:
    Editor (const Volt::DataSource* source);
    virtual ~Editor ();

    virtual void timerEvent (QTimerEvent* event);

    void Render ();

    float dt ();

    void MoveHorizontal (int dir);
    void MoveVertical (int dir);

    void OnModified ();
    void ClearModified ();

    void SetTitle (string title);

    void OnViewportMouseRelease (QMouseEvent* event);
    void OnViewportMouseMove (QMouseEvent* event);
    void OnViewportMousePress (QMouseEvent* event);
    void OnViewportWheel (QWheelEvent* event);

    void SetAppendMode (bool mode) { m_appendMode = mode; }
    void SetRemoveMode (bool mode) { m_removeMode = mode; }

    Vector2 SnapToGrid (Vector2 point);
    float SnapToGrid (float value);
    void SetGridSize (float size) { m_gridSize = size; }
    float GridSize () const { return m_gridSize; }

protected:
    //virtual void mouseMoveEvent (QMouseEvent* event);
    //virtual void mousePressEvent (QMouseEvent* event);
    //virtual void mouseReleaseEvent (QMouseEvent* event);
    //virtual void mouseDoubleClickEvent (QMouseEvent* event);
    virtual void keyPressEvent (QKeyEvent *event);

private slots:
    void New ();
    void Open ();
    bool Save ();
    bool SaveAs ();
    bool Close ();
    void Exit ();
    void Clone ();
    void SelectMode (int id);
    void GridChecked (int state);
    void GridChanged (double value);
    void SnapChecked (int state);
    void OpenRecent ();
    void SelectAll ();
    void Create (QString entityName);
    void PropertyActivated ();

private:
    class ModeState : public Volt::FSMState {
    public:
        explicit ModeState (Editor* e)
            : m_e(e) { }
        Editor* m_e;

        virtual void OnViewportMouseRelease (QMouseEvent* event) { }
        virtual void OnViewportMouseMove (QMouseEvent* event) { }
        virtual void OnViewportMousePress (QMouseEvent* event) { }

        virtual void Render () { }
    private:
        DISALLOW_COPY_AND_ASSIGN(ModeState);
    };

    class PanState : public ModeState {
    public:
        explicit PanState (Editor* e)
            : ModeState(e),
              m_dragging(false) { }
        virtual void OnEnter ();
        virtual void OnExit ();

        virtual void OnViewportMousePress (QMouseEvent* event);
        virtual void OnViewportMouseRelease (QMouseEvent* event);
        virtual void OnViewportMouseMove (QMouseEvent* event);

    private:
        bool m_dragging;
        QPoint m_lastPoint;
    };

    class SelectState : public ModeState {
    public:
        explicit SelectState (Editor* e)
            : ModeState(e),
              m_dragging(false) { }
        virtual void OnEnter ();
        virtual void OnExit ();

        virtual void OnViewportMousePress (QMouseEvent* event);
        virtual void OnViewportMouseMove (QMouseEvent* event);
        virtual void OnViewportMouseRelease (QMouseEvent* event);

        virtual void Render ();
    private:
        bool m_dragging;
        Vector2 m_startPos;
        Vector2 m_currentPos;
    };

    class SelectVerticesState : public ModeState {
    public:
        explicit SelectVerticesState (Editor* e)
            : ModeState(e) { }
        virtual void OnEnter ();
        virtual void OnExit ();

        virtual void OnViewportMousePress (QMouseEvent* event);
        virtual void OnViewportMouseMove (QMouseEvent* event);
    };

    class MoveState : public ModeState {
    public:
        explicit MoveState (Editor* e)
            : ModeState(e),
              m_dragging(false) { }
        virtual void OnEnter ();
        virtual void OnExit ();

        virtual void OnViewportMouseRelease (QMouseEvent* event);
        virtual void OnViewportMousePress (QMouseEvent* event);
        virtual void OnViewportMouseMove (QMouseEvent* event);

    private:
        bool m_dragging;
        Vector2 m_startPoint;
        vector<Entity*> m_selectedEntities;
        vector<Vector2> m_startPositions;
    };

    class RotateState : public ModeState {
    public:
        explicit RotateState (Editor* e)
            : ModeState(e),
              m_dragging(false) { }
        virtual void OnEnter ();
        virtual void OnExit ();

        virtual void OnViewportMouseRelease (QMouseEvent* event);
        virtual void OnViewportMousePress (QMouseEvent* event);
        virtual void OnViewportMouseMove (QMouseEvent* event);

        virtual void Render ();

        Vector2 GetWorldPivotPoint ();
    private:
        bool m_dragging;
        vector<Entity*> m_selectedEntities;
        vector<float> m_startRotations;
        Vector2 m_startPoint;
    };

    class ScaleState : public ModeState {
    public:
        explicit ScaleState (Editor* e)
            : ModeState(e),
              m_dragging(false) { }
        virtual void OnEnter ();
        virtual void OnExit ();

        virtual void OnViewportMouseRelease (QMouseEvent* event);
        virtual void OnViewportMousePress (QMouseEvent* event);
        virtual void OnViewportMouseMove (QMouseEvent* event);

        virtual void Render ();

        Vector2 GetWorldPivotPoint ();
    private:
        bool m_dragging;
        QPoint m_lastPoint;
    };

    void AddRecentDocument (string filename);
    void LoadRecentDocuments ();
    void OpenFile (string filename);

    int CheckModified ();
    Entity* GetTopEntityAtPoint (Vector2 screenPos);
    Triangle* GetTopVertexAtPoint (Vector2 screenPos, int* selectedVertex);

    void Autosave ();

    Volt::AssetManager* m_assetManager;
    Volt::Graphics* m_graphics;
    Volt::PhysicsManager* m_physicsManager;
    GLWidget* m_viewport;
    SelectionManager* m_selectionManager;

    string m_title;
    EditorScene* m_scene;
    bool m_modified;
    Volt::FSM* m_modeFsm;
    PanState* m_panState;
    bool m_panning;
    bool m_appendMode; // Append items or reselect new ones?
    bool m_removeMode; // Remove items or reselect new ones?
    float m_gridSize;
    bool m_gridOn;
    bool m_snapOn;
    PropertyModel* m_propertyModel;
    vector<QAction*> m_recentActions;
    QSettings* m_settings;
    QAction* m_recentFileSeparator;
    QLabel* m_modifiedLabel;
    int m_autosaveTimer;
    int m_updateTimer;
};
