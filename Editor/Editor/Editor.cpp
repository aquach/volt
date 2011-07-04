#include "Editor/Editor/Editor.h"
#include "Editor/Editor/EditorScene.h"
#include "Editor/Editor/GLWidget.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/Viewport.h"

const float SECONDS_PER_UPDATE = 1.0 / 30.0f;

Editor::Editor (const Volt::DataSource* source)
    : m_scene(NULL),
      m_graphics(NULL),
      m_physicsManager(NULL),
      m_viewport(NULL) {
    setWindowTitle("Endmill Editor");
    resize(1024, 768);

    QMenuBar* menu = menuBar();
    QMenu* file = menu->addMenu("&File");
    file->addAction("&Open");
    file->addAction("&Save");
    file->addSeparator();
    file->addAction("&Exit");
    
    m_viewport = new GLWidget;
    m_viewport->makeCurrent();
    setCentralWidget(m_viewport);
    Volt::Viewport::Register(m_viewport);

    statusBar()->showMessage("Ready");

    m_graphics = new Volt::Graphics(m_viewport);
    m_graphics->Set2D(m_viewport->width(), m_viewport->height());
    m_graphics->Init();

    m_assetManager = new Volt::AssetManager(source);
    Volt::AssetManager::Register(m_assetManager);

    m_physicsManager = new Volt::PhysicsManager;
    Volt::PhysicsManager::Register(m_physicsManager);

    m_scene = new EditorScene;

    startTimer((int)(SECONDS_PER_UPDATE * 1000));
}

Editor::~Editor () {
    delete m_graphics;
    delete m_assetManager;
}

void Editor::timerEvent (QTimerEvent* event) {
    UpdateScene();
    m_viewport->update();
}

void Editor::UpdateScene () {
    m_scene->Update();
}

void Editor::RenderScene () {
    m_scene->Render();
}

void Editor::keyPressEvent (QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;
        default:
            event->ignore();
            break;
    }
}
