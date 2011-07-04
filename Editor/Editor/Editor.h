#pragma once

#include "Editor/Core/Core.h"
#include <QMainWindow>
#include <QtGui>

namespace Volt {
    class AssetManager;
    class DataSource;
    class Graphics;
    class PhysicsManager;
}

extern const char* EDITOR_TITLE;

class EditorScene;
class GLWidget;

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

protected:
    virtual void keyPressEvent (QKeyEvent *event);

private slots:
    void New ();
    void Open ();
    bool Save ();
    bool SaveAs ();
    bool Close ();
    void Exit ();

private:
    int CheckModified ();
    Volt::AssetManager* m_assetManager;
    Volt::Graphics* m_graphics;
    Volt::PhysicsManager* m_physicsManager;
    GLWidget* m_viewport;

    EditorScene* m_scene;
    bool m_modified;
};
