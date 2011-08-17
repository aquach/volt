#include "Editor/Editor/Editor.h"
#include <QtGui>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/Viewport.h"
#include "Volt/Python/Python.h"
#include "Game/Game/DoodadManager.h"
#include "Game/Game/LevelManager.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Editor/SelectionManager.h"
#include "Game/Entities/Game/Doodad.h"
#include "Game/Entities/Game/Light.h"
#include "Game/Entities/Game/Triangle.h"
#include "Game/Python/Python.h"
#include "Game/Python/PythonEntityFactory.h"
#include "Editor/Editor/EditorScene.h"
#include "Editor/Editor/GLWidget.h"
#include "Editor/Editor/PropertyModel.h"

const float SECONDS_PER_UPDATE = 1.0 / 30.0f;
const int SECONDS_PER_AUTOSAVE = 60 * 5;
const int MAX_RECENT_DOCUMENTS = 10;

const char* LEVEL_EXTENSION = ".json";

enum ModeButtonType {
    MODE_PAN,
    MODE_SELECT,
    MODE_SELECT_VERTICES,
    MODE_MOVE,
    MODE_ROTATE,
    MODE_SCALE
};

void Editor::EditorSelectionListener::OnEntitySelected (Entity* e) {
    m_e->m_propertyModel->SetEntity(e);
    m_e->m_properties->resizeColumnsToContents();
    m_e->RefreshTags();
}

void Editor::EditorSelectionListener::OnEntityDeselected (Entity* e) {
    m_e->m_propertyModel->SetEntity(NULL);
    m_e->RefreshTags();
}

void Editor::EditorSelectionListener::OnDeselectAll () {
    m_e->m_propertyModel->SetEntity(NULL);
    m_e->RefreshTags();
}

Editor::Editor (const Volt::DataSource* source, int argc, char** argv)
    : m_scene(NULL),
      m_graphics(NULL),
      m_physicsManager(NULL),
      m_viewport(NULL),
      m_panState(NULL),
      m_panning(false),
      m_selectionManager(NULL),
      m_appendMode(false),
      m_removeMode(false),
      m_gridSize(0.5f),
      m_gridOn(true),
      m_snapOn(false),
      m_propertyModel(NULL),
      m_settings(NULL),
      m_recentFileSeparator(NULL),
      m_modifiedLabel(NULL),
      m_updateTimer(0),
      m_autosaveTimer(0),
      m_properties(NULL),
      m_brushesCombo(NULL),
      m_appTime(NULL) {
    setWindowTitle(EDITOR_TITLE);
    resize(1024, 768);
    setMinimumSize(1024, 768);

    m_appTime = new Volt::AppTime;
    m_appTime->SetDt(SECONDS_PER_UPDATE);
    Volt::AppTime::Register(m_appTime);
    
    m_settings = new QSettings;

    m_assetManager = new Volt::AssetManager(source);
    Volt::AssetManager::Register(m_assetManager);

    Volt::Python::Initialize(argc, argv);
    Python::Initialize();

    m_physicsManager = new Volt::PhysicsManager;
    m_physicsManager->SetDebugDraw(true);
    Volt::PhysicsManager::Register(m_physicsManager);

    m_viewport = new GLWidget;
    m_viewport->makeCurrent();
    setCentralWidget(m_viewport);
    Volt::Viewport::Register(m_viewport);

    m_graphics = new Volt::Graphics(m_viewport);
    m_graphics->Set2D(m_viewport->width(), m_viewport->height());
    m_graphics->Init();

    m_scene = new EditorScene;
    m_scene->m_editor = this;

    m_selectionManager = new SelectionManager;
    SelectionManager::Register(m_selectionManager);

    m_selectionListener = new Editor::EditorSelectionListener(this);
    G_SelectionManager->AddSelectionListener(m_selectionListener);

    QAction* action;
    QPushButton* button;
    QShortcut* shortcut;
    QCheckBox* checkbox;
    QDockWidget* dock;
    QComboBox* combo;

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

    QMenu* level = menu->addMenu("&Level");
    action = new QAction("Change &Level Name..", this);
    connect(action, SIGNAL(triggered()), this, SLOT(ChangeLevelName()));
    level->addAction(action);
    action = new QAction("Set Init &Script..", this);
    connect(action, SIGNAL(triggered()), this, SLOT(SetInitScript()));
    level->addAction(action);

    QMenu* editor = menu->addMenu("&Editor");
    action = new QAction("Debug Draw", this);
    action->setCheckable(true);
    action->setChecked(true);
    action->setShortcut(tr("F1"));
    connect(action, SIGNAL(changed()), this, SLOT(ChangeDebugDraw()));
    editor->addAction(action);
    action = new QAction("Reload Brushes", this);
    connect(action, SIGNAL(triggered()), this, SLOT(ReloadBrushes()));
    editor->addAction(action);

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

    QGroupBox* createGroup = new QGroupBox("Create");
    QGridLayout* groupLayout = new QGridLayout;
    combo = new QComboBox(this);
    vector<string> entityTypes;
    EntityFactory::GetEntityTypes(&entityTypes);
    for (uint i = 0; i < entityTypes.size(); i++)
        combo->addItem(QString::fromStdString(entityTypes[i]));
    groupLayout->addWidget(combo, 0, 0);
    connect(combo, SIGNAL(activated(QString)), this,
            SLOT(Create(QString)));

    m_brushesCombo = new QComboBox(this);
    LoadBrushes();
    groupLayout->addWidget(m_brushesCombo, 0, 1);
    connect(m_brushesCombo, SIGNAL(activated(int)), this,
            SLOT(CreateDoodad(int)));

    combo = new QComboBox(this);
    vector<string> pythonEntityNames;
    PythonEntityFactory::GetEntityTypes(
        m_assetManager->sourcePath() + "/Scripts",
        &pythonEntityNames,
        &m_pythonEntityIds);
    for (uint i = 0; i < pythonEntityNames.size(); i++)
        combo->addItem(QString::fromStdString(pythonEntityNames[i]));
    groupLayout->addWidget(combo, 1, 0);
    connect(combo, SIGNAL(activated(int)), this,
            SLOT(CreatePythonEntity(int)));

    createGroup->setLayout(groupLayout);

    layout->addWidget(createGroup);

    button = new QPushButton("Expand Triangle");
    layout->addWidget(button);

    button = new QPushButton("Recompute Static Lightmap");
    connect(button, SIGNAL(clicked()), this, SLOT(RecomputeLightmap()));
    layout->addWidget(button);

    layout->insertStretch(25);

    tools->setLayout(layout);

    dock = new QDockWidget("Tags", this);
    dock->setFeatures(QDockWidget::DockWidgetMovable |
                      QDockWidget::DockWidgetFloatable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    QWidget* tags = new QWidget;
    tags->setMaximumHeight(120);
    dock->setWidget(tags);
    layout = new QVBoxLayout;
    tags->setLayout(layout);

    m_tagList = new QListWidget;
    layout->addWidget(m_tagList);
    connect(m_tagList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
            SLOT(RemoveTag(QListWidgetItem*)));

    m_tagEdit = new QLineEdit;
    layout->addWidget(m_tagEdit);
    connect(m_tagEdit, SIGNAL(returnPressed()), this, SLOT(AddTag()));


    dock = new QDockWidget("Properties", this);
    dock->setFeatures(QDockWidget::DockWidgetMovable |
                      QDockWidget::DockWidgetFloatable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    m_properties = new QTableView;
    m_propertyModel = new PropertyModel;
    m_properties->setModel(m_propertyModel);
    m_properties->horizontalHeader()->setStretchLastSection(true);
    connect(m_properties, SIGNAL(clicked(QModelIndex)), this,
            SLOT(PropertyActivated(QModelIndex)));
    dock->setWidget(m_properties);

    statusBar()->showMessage("Ready");
    m_fpsLabel = new QLabel("");
    m_modifiedLabel = new QLabel("");
    statusBar()->addPermanentWidget(m_fpsLabel);
    statusBar()->addPermanentWidget(m_modifiedLabel);

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

    m_updateTimer = startTimer((int)(SECONDS_PER_UPDATE * 1000));
    m_autosaveTimer = startTimer((int)(SECONDS_PER_AUTOSAVE * 1000));
}

void Editor::LoadBrushes () {
    m_brushesCombo->clear();
    vector<DoodadBrush*> brushes;
    G_DoodadManager->GetDoodadBrushes(&brushes);
    for (uint i = 0; i < brushes.size(); i++)
        m_brushesCombo->addItem(QString::fromStdString(brushes[i]->name));
}

void Editor::ReloadBrushes () {
    G_DoodadManager->ReloadBrushes();
    LoadBrushes();
}

Editor::~Editor () {
    G_SelectionManager->RemoveSelectionListener(m_selectionListener);
    delete m_selectionListener;
    delete m_selectionManager;
    delete m_propertyModel;
    delete m_scene;
    delete m_graphics;
    delete m_assetManager;
}

void Editor::timerEvent (QTimerEvent* event) {
    if (event->timerId() == m_updateTimer) {
        m_scene->Update();
        m_viewport->update();
    } else if (event->timerId() == m_autosaveTimer) {
        Autosave();
    }
}

void Editor::MoveHorizontal (int dir) {
    m_scene->MoveHorizontal(dir);
}

void Editor::MoveVertical (int dir) {
    m_scene->MoveVertical(dir);
}

void Editor::Render () {
    QTime t;
    t.start();

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
        Graphics::SetBlend(Graphics::BLEND_NONE);
    }

    // Render GUI elements that the mode might want drawn.
    Editor::ModeState* state;
    state = dynamic_cast<Editor::ModeState*>(m_modeFsm->state());
    CHECK_NOTNULL(state);
    state->Render();

    if (t.elapsed() > 5) {
        m_fpsLabel->setText(tr("FPS: %1").arg((int)(1000.0f / t.elapsed())));
    } else {
        m_fpsLabel->setText("FPS: High");
    }
}

void Editor::Screenshot () {
    QImage image = m_viewport->grabFrameBuffer();
    int hour, min, sec;
    long usec;
    Volt::GetTimestamp(&hour, &min, &sec, &usec);
    QString filename =
        tr("screenshot_%1_%2_%3.png").arg(hour).arg(min).arg(sec);
    bool success = image.save(filename);
    if (success) {
        statusBar()->showMessage(tr("Saved screenshot to %1").arg(filename));
    } else {
        statusBar()->showMessage("Failed to save screenshot.");

    }
}

void Editor::keyPressEvent (QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            Exit();
            break;
        case Qt::Key_Delete:
            Delete();
        break;
        case Qt::Key_F2:
            // Reload all shaders.
            G_AssetManager->ReloadShaders();
        break;
        case Qt::Key_F9:
            Screenshot();
        break;
        default:
            event->ignore();
        break;
    }
}

void Editor::SetTitle (const string& title) {
    m_title = title;
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
    UpdateTitle();
    OnModified();
}

void Editor::Open () {
    if (!Close())
        return;

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

void Editor::OpenFile (const string& filename) {
    bool success = m_scene->m_levelManager->LoadLevelFromFilename(
                    filename);
    if (success) {
        AddRecentDocument(filename);
        statusBar()->showMessage("Opened level.");
        ClearModified();
        UpdateTitle();
    } else {
        QMessageBox::critical(this, " ", "Failed to open file.");
    }
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

    if (!filename.contains('.'))
        filename += LEVEL_EXTENSION;

    bool success = m_scene->m_levelManager->SaveLevel(filename.toStdString());
    if (success) {
        statusBar()->showMessage("Saved level.");
        ClearModified();
        AddRecentDocument(filename.toStdString());
        UpdateTitle();
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
    G_SelectionManager->DeselectAll();
    m_scene->m_levelManager->UnloadLevel();
    m_scene->RemoveAll();
    UpdateTitle();
    ClearModified();
    return true;
}

void Editor::Exit () {
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
    OnModified();
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

void Editor::AddRecentDocument (const string& filename) {
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
    if (!Close())
        return;

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

void Editor::Create (QString entityName) {
    OnModified();
    Entity* e = EntityFactory::Create(entityName.toStdString());
    m_scene->Add(e);
    OnModified();
}

void Editor::PropertyActivated (const QModelIndex& index) {
    m_propertyModel->OnPropertyActivated(index);
}

void Editor::OnModified () {
    m_modified = true;
    m_modifiedLabel->setText("Modified");
}

void Editor::ClearModified () {
    m_modified = false;
    m_modifiedLabel->setText("");
}

void Editor::Autosave () {
    string loadedFile = m_scene->m_levelManager->loadedFile();
    if (m_modified && loadedFile != "") {
        QString name = QString::fromStdString(loadedFile);
        if (!name.endsWith(LEVEL_EXTENSION))
            name += LEVEL_EXTENSION;

        int backup = 1;
        QString filename;
        while (true) {
            filename = name;
            filename = filename.replace(".",
                                        QString("_autobak_%1.").arg(backup));
            if (!QFileInfo(filename).exists())
                break;
            backup++;
        }
        bool success = m_scene->m_levelManager->SaveLevel(
                            filename.toStdString());
        if (success) {
            LOG(INFO) << "Autosaved level " << filename.toStdString();
            statusBar()->showMessage(
                QString("Autosaved level %1.").arg(filename));
        } else {
            LOG(ERROR) << "Failed to autosave level " << filename.toStdString();
            QMessageBox::critical(this, " ",
                QString("Failed to autosave file %1.").arg(filename));
        }
    }
}

void Editor::CreateDoodad (int index) {
    // TODO: Inefficient.
    vector<DoodadBrush*> brushes;
    G_DoodadManager->GetDoodadBrushes(&brushes);
    Doodad* d = new Doodad;
    d->SetBrush(G_DoodadManager->GetDoodadBrush(brushes[index]->id));
    m_scene->Add(d);
    OnModified();
}

void Editor::ChangeLevelName () {
    bool ok;
    QString text = QInputDialog::getText(
        this,
        tr("Change Level Name"),
        tr("Level Name"),
        QLineEdit::Normal,
        QString::fromStdString(m_scene->m_levelManager->levelName()),
        &ok);

    if (ok && !text.isEmpty()) {
        m_scene->m_levelManager->SetLevelName(text.toStdString());
        OnModified();
        UpdateTitle();
    }
}

void Editor::UpdateTitle () {
    string filename = m_scene->m_levelManager->loadedFile();
    if (filename != "") {
        SetTitle(filename + " - " +
                 m_scene->m_levelManager->levelName());
    } else {
        SetTitle("");
    }
}

void Editor::closeEvent (QCloseEvent* event) {
    int ret = CheckModified();
    switch (ret) {
        case QMessageBox::Save:
            if (!Save()) {
                event->ignore();
                return;
            }
        break;
        case QMessageBox::Discard:
        break;
        case QMessageBox::Cancel:
            event->ignore();
            return;
        break;
        default:
        break;
    }
    event->accept();
}

void Editor::ChangeDebugDraw () {
    QAction* action = qobject_cast<QAction*>(sender());
    m_physicsManager->SetDebugDraw(action->isChecked());
}

void Editor::Delete () {
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);
    if (selectedEntities.size() == 0)
        return;
    G_SelectionManager->DeselectAll();
    OnModified();
    for (uint i = 0; i < selectedEntities.size(); i++) {
        m_scene->Remove(selectedEntities[i]);
    }
}

void Editor::RecomputeLightmap () {
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);
    if (selectedEntities.size() == 0)
        return;
    for (uint i = 0; i < selectedEntities.size(); i++) {
        if (Light* l = dynamic_cast<Light*>(selectedEntities[i])) {
            l->InvalidateStaticMap();
        }
    }
    OnModified();
}

void Editor::SetInitScript () {
    bool ok;
    QString text = QInputDialog::getText(
        this,
        tr("Set Init Script"),
        tr("Init Script"),
        QLineEdit::Normal,
        QString::fromStdString(m_scene->m_levelManager->startScript()),
        &ok);

    if (ok && !text.isEmpty()) {
        m_scene->m_levelManager->SetStartScript(text.toStdString());
        OnModified();
    }
}

void Editor::AddTag () {
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);
    string tag = m_tagEdit->text().toStdString();
    for (uint i = 0; i < selectedEntities.size(); i++)
        selectedEntities[i]->AddTag(tag);
    RefreshTags();
    m_tagEdit->setText("");
}

void Editor::RemoveTag (QListWidgetItem* item) {
    vector<Entity*> selectedEntities;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);
    string tag = item->text().toStdString();
    for (uint i = 0; i < selectedEntities.size(); i++)
        selectedEntities[i]->RemoveTag(tag);
    RefreshTags();
}

void Editor::RefreshTags () {
    m_tagList->clear();
    vector<Entity*> selectedEntities;
    set<string> tags;
    G_SelectionManager->GetSelectedEntities(&selectedEntities);
    for (uint i = 0; i < selectedEntities.size(); i++) {
        vector<string> entityTags;
        selectedEntities[i]->GetTags(&entityTags);
        copy(entityTags.begin(), entityTags.end(), inserter(tags, tags.end()));
    }
    FOR_(set<string>::iterator, i, tags) {
        m_tagList->addItem(QString::fromStdString(*i));
    }
}

void Editor::CreatePythonEntity (int index) {
    Entity* e = PythonEntityFactory::CreateEntity(m_pythonEntityIds[index]);
    m_scene->Add(e);
    OnModified();
}
