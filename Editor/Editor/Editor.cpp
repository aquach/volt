#include "Editor/Editor/Editor.h"
#include <QtGui>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/Viewport.h"
#include "Game/Game/LevelManager.h"
#include "Editor/Editor/EditorScene.h"
#include "Editor/Editor/GLWidget.h"

const float SECONDS_PER_UPDATE = 1.0 / 30.0f;

enum ModeButtonType {
    MODE_PAN,
    MODE_SELECT,
    MODE_SELECT_VERTICES
};

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
}

void Editor::SelectState::OnViewportMouseMove (QMouseEvent* event) {
}
        
Editor::Editor (const Volt::DataSource* source)
    : m_scene(NULL),
      m_graphics(NULL),
      m_physicsManager(NULL),
      m_viewport(NULL),
      m_panState(NULL),
      m_panning(false) {
    setWindowTitle(EDITOR_TITLE);
    resize(1024, 768);
    setMinimumSize(1024, 768);

    QAction* action;
    QPushButton* button;

    QMenuBar* menu = menuBar();
    QMenu* file = menu->addMenu("&File");
    action = new QAction("&New", this);
    connect(action, SIGNAL(triggered()), this, SLOT(New()));
    file->addAction(action);
    action = new QAction("&Open", this);
    connect(action, SIGNAL(triggered()), this, SLOT(Open()));
    action->setShortcut(tr("Ctrl+O"));
    file->addAction(action);
    action = new QAction("&Save", this);
    action->setShortcut(tr("Ctrl+S"));
    connect(action, SIGNAL(triggered()), this, SLOT(Save()));
    file->addAction(action);
    action = new QAction("&Save As..", this);
    connect(action, SIGNAL(triggered()), this, SLOT(SaveAs()));
    file->addAction(action);
    action = new QAction("&Close", this);
    connect(action, SIGNAL(triggered()), this, SLOT(Close()));
    action->setShortcut(tr("Ctrl+W"));
    file->addAction(action);
    file->addSeparator();
    action = new QAction("&Exit", this);
    connect(action, SIGNAL(triggered()), this, SLOT(Exit()));
    file->addAction(action);

    QToolBar* toolbar = addToolBar(tr("File"));
    QButtonGroup* group;
    group = new QButtonGroup;
    button = new QPushButton("Pan");
    button->setCheckable(true);
    group->addButton(button);
    group->setId(button, MODE_PAN);
    toolbar->addWidget(button);
    button = new QPushButton("Select");
    button->setCheckable(true);
    group->addButton(button);
    group->setId(button, MODE_SELECT);
    toolbar->addWidget(button);
    button = new QPushButton("Select Vertices");
    button->setCheckable(true);
    group->addButton(button);
    group->setId(button, MODE_SELECT_VERTICES);
    toolbar->addWidget(button);
    connect(group, SIGNAL(buttonClicked(int)), this, SLOT(SelectMode(int)));

    toolbar->addSeparator();

    QDockWidget* dock = new QDockWidget("Tools", this);
    dock->setFeatures(QDockWidget::DockWidgetMovable |
                      QDockWidget::DockWidgetFloatable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    QWidget* tools = new QWidget;
    dock->setWidget(tools);
    QVBoxLayout* layout = new QVBoxLayout;

    button = new QPushButton("New Triangle");
    layout->addWidget(button);
    button = new QPushButton("Expand Triangle");
    layout->addWidget(button);
    layout->insertStretch(25);

    tools->setLayout(layout);

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
    m_scene->m_editor = this;

    m_modeFsm = new Volt::FSM;
    m_panState = new Editor::PanState(this);
    m_modeFsm->AddState(m_panState, "PanMode");
    m_modeFsm->AddState(new Editor::SelectState(this), "SelectMode");
    /*
    m_modeFsm->AddState(new SelectVerticesModeState(this),
                        "SelectVerticesMode");
    */
    m_modeFsm->TransitionTo("PanMode");
    
    startTimer((int)(SECONDS_PER_UPDATE * 1000));
}

Editor::~Editor () {
    delete m_graphics;
    delete m_assetManager;
}

float Editor::dt () {
    return SECONDS_PER_UPDATE;
}

void Editor::timerEvent (QTimerEvent* event) {
    m_scene->Update();
    m_viewport->update();
}

void Editor::MoveHorizontal (int dir) {
    m_scene->MoveHorizontal(dir);
}

void Editor::MoveVertical (int dir) {
    m_scene->MoveVertical(dir);
}

void Editor::RenderScene () {
    m_scene->Render();
}

void Editor::keyPressEvent (QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            Exit();
            break;
        default:
            event->ignore();
            break;
    }
}

void Editor::SetTitle (string title) {
    QString str;
    str += EDITOR_TITLE;
    if (title.size() > 0) {
        str += " (";
        str += title.c_str();
        str += ")";
    }
    setWindowTitle(str);
}

void Editor::New () {
    if (m_scene->m_levelManager->loadedFile() != "") {
        if (!Close())
            return;
    }
    SetTitle("New Level");
    OnModified();
}

void Editor::Open () {
    if (m_modified) {
        if (!Close())
            return;
    }

    QString filename = QFileDialog::getOpenFileName(
        this,
        "Open Level",
        ".",
        "JSON files (*.json)"
    );
    if (filename == "")
        return;

    bool success = m_scene->m_levelManager->LoadLevelFromFilename(
                    filename.toStdString());
    if (success) {
        statusBar()->showMessage("Opened level.");
        ClearModified();
    } else {
        QMessageBox::critical(this, " ", "Failed to open file.");
    }
    SetTitle(filename.toStdString());
}

bool Editor::Save () {
    if (m_scene->m_levelManager->loadedFile() == "") {
        return SaveAs();
    }

    bool success = m_scene->m_levelManager->SaveLevel(
                        m_scene->m_levelManager->loadedFile());
    if (success) {
        statusBar()->showMessage("Saved level.");
        ClearModified();
    } else {
        QMessageBox::critical(this, " ", "Failed to save file.");
    }
    return success;
}

bool Editor::SaveAs () {
    QString filename = QFileDialog::getSaveFileName(this, "Save As", ".");
    if (filename == "")
        return true;

    bool success = m_scene->m_levelManager->SaveLevel(filename.toStdString());
    if (success) {
        statusBar()->showMessage("Saved level.");
        ClearModified();
    } else {
        QMessageBox::critical(this, " ", "Failed to save file.");
    }
    return success;
}

int Editor::CheckModified () {
    if (!m_modified)
        return QMessageBox::Discard;

    QMessageBox msgBox;
    msgBox.setWindowTitle(" ");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                              QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

bool Editor::Close () {
    int ret = CheckModified();
    switch (ret) {
        case QMessageBox::Save:
            if (!Save())
                return false;
        break;
        case QMessageBox::Discard:
        break;
        case QMessageBox::Cancel:
            return false;
        break;
        default:
        break;
    }
    m_scene->m_levelManager->UnloadLevel();
    SetTitle("");
    return true;
}

void Editor::Exit () {
    int ret = CheckModified();
    switch (ret) {
        case QMessageBox::Save:
            if (!Save())
                return;
        break;
        case QMessageBox::Discard:
        break;
        case QMessageBox::Cancel:
            return;
        break;
        default:
        break;
    }
    close();
}

void Editor::SelectMode (int id) {
    switch (id) {
        case MODE_PAN:
            m_modeFsm->TransitionTo("PanMode");
        break;
        case MODE_SELECT:
            m_modeFsm->TransitionTo("SelectMode");
        break;
        case MODE_SELECT_VERTICES:
            m_modeFsm->TransitionTo("SelectVerticesMode");
        break;
        default:
            LOG(ERROR) << "Invalid select mode " << id;
        break;
    }
}

void Editor::OnViewportMouseRelease (QMouseEvent* event) {
    Editor::ModeState* state;
    state = dynamic_cast<Editor::ModeState*>(m_modeFsm->state());
    CHECK_NOTNULL(state);
    if (event->button() == Qt::MiddleButton) {
        // Redirect to pan mode.
        m_panState->OnViewportMouseRelease(event);
        if (state != m_panState)
            m_panState->OnExit();
    } else {
        state->OnViewportMouseRelease(event);
    }
}

void Editor::OnViewportMouseMove (QMouseEvent* event) {
    if (m_panning) {
        // Redirect to pan mode.
        m_panState->OnViewportMouseMove(event);
    } else {
        Editor::ModeState* state;
        state = dynamic_cast<Editor::ModeState*>(m_modeFsm->state());
        CHECK_NOTNULL(state);
        state->OnViewportMouseMove(event);
    }
}

void Editor::OnViewportMousePress (QMouseEvent* event) {
    Editor::ModeState* state;
    state = dynamic_cast<Editor::ModeState*>(m_modeFsm->state());
    CHECK_NOTNULL(state);
    if (event->button() == Qt::MiddleButton) {
        // Redirect to pan mode. Bit of a hack.
        if (m_panState != state)
            m_panState->OnEnter();
        m_panState->OnViewportMousePress(event);
        m_panning = true;
    } else {
        state->OnViewportMousePress(event);
    }
}

void Editor::OnViewportWheel (QWheelEvent* event) {
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    m_scene->camera()->transform.scale *= pow(1.2, numSteps);
    event->accept();
}
