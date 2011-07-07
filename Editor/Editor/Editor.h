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
class GLWidget;
class SelectionManager;

class Editor : public QMainWindow {
    Q_OBJECT;

public:
    Editor (const Volt::DataSource* source);
    virtual ~Editor ();

    virtual void timerEvent (QTimerEvent* event);

    void RenderScene ();

    float dt ();

    void MoveHorizontal (int dir);
    void MoveVertical (int dir);

    void OnModified () { m_modified = true; }
    void ClearModified () { m_modified = false; }

    void SetTitle (string title);

    void OnViewportMouseRelease (QMouseEvent* event);
    void OnViewportMouseMove (QMouseEvent* event);
    void OnViewportMousePress (QMouseEvent* event);
    void OnViewportWheel (QWheelEvent* event);

    void SetAppendMode (bool mode) { m_appendMode = mode; }

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
    void SelectMode (int id);

private:
    class ModeState : public Volt::FSMState {
    public:
        explicit ModeState (Editor* e)
            : m_e(e) { }
        Editor* m_e;

        virtual void OnViewportMouseRelease (QMouseEvent* event) { };
        virtual void OnViewportMouseMove (QMouseEvent* event) { };
        virtual void OnViewportMousePress (QMouseEvent* event) { };
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
            : ModeState(e) { }
        virtual void OnEnter ();
        virtual void OnExit ();

        virtual void OnViewportMousePress (QMouseEvent* event);
        virtual void OnViewportMouseMove (QMouseEvent* event);
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

    int CheckModified ();

    Volt::AssetManager* m_assetManager;
    Volt::Graphics* m_graphics;
    Volt::PhysicsManager* m_physicsManager;
    GLWidget* m_viewport;
    SelectionManager* m_selectionManager;

    EditorScene* m_scene;
    bool m_modified;
    Volt::FSM* m_modeFsm;
    PanState* m_panState;
    bool m_panning;
    bool m_appendMode; // Append/remove items or reselect new ones?
};
