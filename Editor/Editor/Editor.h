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

protected:
    virtual void keyPressEvent (QKeyEvent *event);

private slots:
    void Open ();
    void Save (); 
    void Exit ();
    
private:
    Volt::AssetManager* m_assetManager;
    Volt::Graphics* m_graphics;
    Volt::PhysicsManager* m_physicsManager;
    GLWidget* m_viewport;
    
    EditorScene* m_scene;
};
