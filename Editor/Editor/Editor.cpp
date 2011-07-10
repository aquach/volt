#include "Editor/Editor/Editor.h"
#include <QtGui>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/Viewport.h"
#include "Game/Game/LevelManager.h"
#include "Game/Editor/SelectionManager.h"
#include "Game/Entities/Game/Triangle.h"
#include "Editor/Editor/EditorScene.h"
#include "Editor/Editor/GLWidget.h"
#include "Editor/Editor/PropertyModel.h"

const float SECONDS_PER_UPDATE = 1.0 / 30.0f;
const int MAX_RECENT_DOCUMENTS = 10;

enum ModeButtonType {
    MODE_PAN,
    MODE_SELECT,
    MODE_SELECT_VERTICES,
    MODE_MOVE,
    MODE_ROTATE,
    MODE_SCALE
};

Editor::Editor (const Volt::DataSource* source)
    : m_scene(NULL),
      m_graphics(NULL),
      m_physicsManager(NULL),
      m_viewport(NULL),
      m_panState(NULL),
      m_panning(false),
      m_selectionManager(NULL),
      m_appendMode(false),
      m_removeMode(false),
      m_gridSize(1.0f),
      m_gridOn(true),
      m_snapOn(false),
      m_propertyModel(NULL),
      m_settings(NULL),
      m_recentFileSeparator(NULL) {
    setWindowTitle(EDITOR_TITLE);
    resize(1024, 768);
    setMinimumSize(1024, 768);

    m_settings = new QSettings;

    QAction* action;
    QPushButton* button;
    QShortcut* shortcut;
    QCheckBox* checkbox;
    QDockWidget* dock;

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

    file->addSeparator();

    for (int i = 0; i < MAX_RECENT_DOCUMENTS; i++) {
        action = new QAction("", this);
        connect(action, SIGNAL(triggered()), this, SLOT(OpenRecent()));
        file->addAction(action);
        m_recentActions.push_back(action);
    }
    m_recentFileSeparator = file->addSeparator();

    LoadRecentDocuments();

    action = new QAction("Close", this);
    connect(action, SIGNAL(triggered()), this, SLOT(Close()));
    action->setShortcut(tr("Ctrl+W"));
    file->addAction(action);
    file->addSeparator();
    action = new QAction("&Exit", this);
    connect(action, SIGNAL(triggered()), this, SLOT(Exit()));
    file->addAction(action);

    QMenu* edit = menu->addMenu("&Edit");
   action = new QAction("Select All", this);
    action->setShortcut(tr("Ctrl+A"));
    connect(action, SIGNAL(triggered()), this, SLOT(SelectAll()));
    edit->addAction(action);
    action = new QAction("Clone", this);
    action->setShortcut(tr("Ctrl+D"));
    connect(action, SIGNAL(triggered()), this, SLOT(Clone()));
    edit->addAction(action);

    QToolBar* toolbar = addToolBar(tr("Modes"));
    QButtonGroup* group;
    group = new QButtonGroup;
    button = new QPushButton("Pan");
    button->setCheckable(true);
    group->addButton(button);
    group->setId(button, MODE_PAN);
    toolbar->addWidget(button);
    shortcut = new QShortcut(QKeySequence("1"), this);
    connect(shortcut, SIGNAL(activated()), button, SLOT(click()));

    button = new QPushButton("Select");
    button->setCheckable(true);
    group->addButton(button);
    group->setId(button, MODE_SELECT);
    toolbar->addWidget(button);
    shortcut = new QShortcut(QKeySequence("2"), this);
    connect(shortcut, SIGNAL(activated()), button, SLOT(click()));

    button = new QPushButton("Select Vertices");
    button->setCheckable(true);
    group->addButton(button);
    group->setId(button, MODE_SELECT_VERTICES);
    toolbar->addWidget(button);
    shortcut = new QShortcut(QKeySequence("3"), this);
    connect(shortcut, SIGNAL(activated()), button, SLOT(click()));

    toolbar->addSeparator();

    button = new QPushButton("Move");
    button->setCheckable(true);
    group->addButton(button);
    group->setId(button, MODE_MOVE);
    toolbar->addWidget(button);
    shortcut = new QShortcut(QKeySequence("W"), this);
    connect(shortcut, SIGNAL(activated()), button, SLOT(click()));

    button = new QPushButton("Rotate");
    button->setCheckable(true);
    group->addButton(button);
    group->setId(button, MODE_ROTATE);
    toolbar->addWidget(button);
    shortcut = new QShortcut(QKeySequence("E"), this);
    connect(shortcut, SIGNAL(activated()), button, SLOT(click()));

    /*
    button = new QPushButton("Scale");
    button->setCheckable(true);
    group->addButton(button);
    group->setId(button, MODE_SCALE);
    toolbar->addWidget(button);
    shortcut = new QShortcut(QKeySequence("R"), this);
    connect(shortcut, SIGNAL(activated()), button, SLOT(click()));
    */

    checkbox = new QCheckBox("&Global", this);
    toolbar->addWidget(checkbox);

    toolbar->addSeparator();

    checkbox = new QCheckBox("&Snap", this);
    checkbox->setChecked(m_snapOn);
    toolbar->addWidget(checkbox);
    connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(SnapChecked(int)));

    checkbox = new QCheckBox("Gri&d", this);
    checkbox->setChecked(m_gridOn);
    toolbar->addWidget(checkbox);
    connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(GridChecked(int)));

    QDoubleSpinBox* spinbox = new QDoubleSpinBox(this);
    spinbox->setMinimum(0.1f);
    spinbox->setSingleStep(0.1f);
    spinbox->setDecimals(1);
    spinbox->setValue(m_gridSize);
    toolbar->addWidget(spinbox);
    connect(spinbox, SIGNAL(valueChanged(double)), this,
            SLOT(GridChanged(double)));

    connect(group, SIGNAL(buttonClicked(int)), this, SLOT(SelectMode(int)));

    dock = new QDockWidget("Tools", this);
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

    dock = new QDockWidget("Properties", this);
    dock->setFeatures(QDockWidget::DockWidgetMovable |
                      QDockWidget::DockWidgetFloatable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    QTableView* view = new QTableView;
    m_propertyModel = new PropertyModel;
    view->setModel(m_propertyModel);
    view->horizontalHeader()->setStretchLastSection(true);
    dock->setWidget(view);

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
    m_physicsManager->SetDebugDraw(true);
    Volt::PhysicsManager::Register(m_physicsManager);

    m_scene = new EditorScene;
    m_scene->m_editor = this;

    m_modeFsm = new Volt::FSM;
    m_panState = new Editor::PanState(this);
    m_modeFsm->AddState(m_panState, "PanMode");
    m_modeFsm->AddState(new Editor::SelectState(this), "SelectMode");
    m_modeFsm->AddState(new Editor::SelectVerticesState(this),
                        "SelectVerticesMode");
    m_modeFsm->AddState(new Editor::MoveState(this), "MoveMode");
    m_modeFsm->AddState(new Editor::RotateState(this), "RotateMode");
    m_modeFsm->AddState(new Editor::ScaleState(this), "ScaleMode");
    m_modeFsm->TransitionTo("PanMode");

    m_selectionManager = new SelectionManager;
    SelectionManager::Register(m_selectionManager);

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

void Editor::Render () {
    m_scene->Render();

    // Render grid.
    if (m_gridOn) {
        Vector2 upperLeft = m_scene->camera()->ScreenToWorld(Vector2(0, 0));
        Vector2 lowerRight = m_scene->camera()->ScreenToWorld(
            Vector2(m_viewport->width(), m_viewport->height()));

        glLineWidth(1.0f);
        Graphics::SetBlend(Graphics::BLEND_ALPHA);
        Graphics::SetColor(Volt::Color::RGBA(200, 200, 200, 100));
        for (float px = upperLeft.x; px < lowerRight.x; px += m_gridSize) {
            float lx = SnapToGrid(px);
            Graphics::RenderLine(Vector2(lx, upperLeft.y),
                                 Vector2(lx, lowerRight.y));
        }
        for (float py = upperLeft.y; py < lowerRight.y; py += m_gridSize) {
            float ly = SnapToGrid(py);
            Graphics::RenderLine(Vector2(upperLeft.x, ly),
                                 Vector2(lowerRight.x, ly));
        }
    }

    // Render GUI elements that the mode might want drawn.
    Editor::ModeState* state;
    state = dynamic_cast<Editor::ModeState*>(m_modeFsm->state());
    CHECK_NOTNULL(state);
    state->Render();
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

    OpenFile(filename.toStdString());
}

void Editor::OpenFile (string filename) {
    bool success = m_scene->m_levelManager->LoadLevelFromFilename(
                    filename);
    if (success) {
        AddRecentDocument(filename);
        statusBar()->showMessage("Opened level.");
        ClearModified();
    } else {
        QMessageBox::critical(this, " ", "Failed to open file.");
    }
    SetTitle(filename);
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
        case MODE_MOVE:
            m_modeFsm->TransitionTo("MoveMode");
        break;
        case MODE_ROTATE:
            m_modeFsm->TransitionTo("RotateMode");
        break;
        case MODE_SCALE:
            m_modeFsm->TransitionTo("ScaleMode");
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
        m_panning = false;
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

Entity* Editor::GetTopEntityAtPoint (Vector2 screenPos) {
    Vector2 pos = m_scene->camera()->ScreenToWorld(screenPos);
    vector<Volt::Entity*> entities;
    m_scene->GetEntitiesAtPoint(pos, &entities);
    if (entities.size() == 0)
        return NULL;

    Volt::Entity* selectedEntity = entities[0];
    for (uint i = 1; i < entities.size(); i++) {
        if (entities[i]->layer() < selectedEntity->layer()) {
            selectedEntity = entities[i];
        }
    }

    return dynamic_cast<Entity*>(selectedEntity);
}

Triangle* Editor::GetTopVertexAtPoint (Vector2 screenPos, int* selectedVertex) {
    Vector2 pos = m_scene->camera()->ScreenToWorld(screenPos);
    vector<Volt::Entity*> entities;
    m_scene->GetEntitiesAtPoint(pos, &entities);

    if (entities.size() == 0)
        return NULL;

    Triangle* selectedTriangle = NULL;
    for (uint i = 0; i < entities.size(); i++) {
        Triangle* tri = dynamic_cast<Triangle*>(entities[i]);
        if (tri == NULL)
            continue;

        if (selectedTriangle == NULL ||
            tri->layer() < selectedTriangle->layer()) {
            selectedTriangle = tri;
        }
    }

    if (selectedTriangle == NULL)
        return NULL;

    *selectedVertex = selectedTriangle->selectedVertex(pos);
    if (*selectedVertex == -1)
        return NULL;

    return selectedTriangle;
}

Vector2 Editor::SnapToGrid (Vector2 point) {
    return Vector2(SnapToGrid(point.x), SnapToGrid(point.y));
}

float Editor::SnapToGrid (float value) {
    return Volt::RoundToNearest(value, m_gridSize);
}

void Editor::GridChecked (int state) {
    m_gridOn = state == Qt::Checked;
}

void Editor::GridChanged (double value) {
    m_gridSize = value;
}

void Editor::SnapChecked (int state) {
    m_snapOn = state == Qt::Checked;
}

void Editor::Clone () {
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);
    G_SelectionManager->DeselectAll();
    FOR_(vector<Entity*>::iterator, i, selectedEntities) {
        Entity* entity = *i;
        Volt::Entity* cloned = entity->Clone();
        if (cloned != NULL) {
            m_scene->Add(cloned, entity->layer());
            if (Entity* e = dynamic_cast<Entity*>(cloned))
                G_SelectionManager->SelectEntity(e);
        }
    }
}

void Editor::AddRecentDocument (string filename) {
    QStringList files = m_settings->value("recentFileList").toStringList();
    files.removeAll(QString::fromStdString(filename));
    files.prepend(QString::fromStdString(filename));
    m_settings->setValue("recentFileList", files);
    LoadRecentDocuments();
}

void Editor::LoadRecentDocuments () {
    QStringList files = m_settings->value("recentFileList").toStringList();
    int numFiles = MIN(files.size(), MAX_RECENT_DOCUMENTS);
    for (int i = 0; i < numFiles; i++) {
        QString text = tr("&%1 %2").arg(i + 1)
                       .arg(QFileInfo(files[i]).fileName());
        m_recentActions[i]->setText(text);
        m_recentActions[i]->setData(QFileInfo(files[i]).absoluteFilePath());
        m_recentActions[i]->setVisible(true);
    }
    for (int i = numFiles; i < MAX_RECENT_DOCUMENTS; i++)
        m_recentActions[i]->setVisible(false);

    m_recentFileSeparator->setVisible(numFiles > 0);
}

void Editor::OpenRecent () {
    QAction* action = qobject_cast<QAction*>(sender());
    CHECK_NOTNULL(action);
    OpenFile(action->data().toString().toStdString());
}

void Editor::SelectAll () {
    Volt::Scene::Layers entitiesByLayer = m_scene->GetEntities();
        FOR_ (Volt::Scene::Layers::iterator, layer, entitiesByLayer) {
            FOR_(list<Volt::Entity*>::iterator, i, layer->second) {
                Entity* e = dynamic_cast<Entity*>(*i);
                if (e != NULL)
                    G_SelectionManager->SelectEntity(e);
            }
        }
}
