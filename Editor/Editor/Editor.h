#pragma once

#include <QMainWindow>

namespace Volt {
    class AssetManager;
    class DataSource;
    class Graphics;
}

class EditorScene;
class GLWidget;

class Editor : public QMainWindow {
    Q_OBJECT;

public:
    Editor (const Volt::DataSource* source);
    virtual ~Editor ();

    virtual void timerEvent (QTimerEvent* event);

    void UpdateScene ();
    void RenderScene ();

private:
    Volt::AssetManager* m_assetManager;
    Volt::Graphics* m_graphics;
    GLWidget* m_viewport;
    
    EditorScene* m_scene;
};
