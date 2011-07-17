#pragma once

#include "Volt/Game/Scene.h"

class DoodadManager;
class Editor;
class LevelManager;
class LightManager;

class EditorScene : public Volt::Scene {
public:
    EditorScene ();
    virtual ~EditorScene ();

    virtual void Update ();

    virtual void OnPostRender ();

    virtual void OnBegin ();
    virtual void OnEnd ();

    void MoveHorizontal (int dir) { m_cameraMoveX = dir; }
    void MoveVertical (int dir) { m_cameraMoveY = dir; }

private:
    friend class Editor;
    Editor* m_editor;
    LevelManager* m_levelManager;
    LightManager* m_lightManager;
    DoodadManager* m_doodadManager;

    int m_cameraMoveX;
    int m_cameraMoveY;
};
