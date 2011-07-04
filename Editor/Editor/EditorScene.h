#pragma once

#include "Volt/Game/Scene.h"

class LevelManager;

class EditorScene : public Volt::Scene {
public:
    EditorScene ();
    virtual ~EditorScene ();

    //virtual void Render ();
    virtual void Update ();

    virtual void OnBegin ();
    virtual void OnEnd ();

    void MoveHorizontal (int dir) { m_cameraMoveX = dir; }
    void MoveVertical (int dir) { m_cameraMoveY = dir; }

private:
    LevelManager* m_levelManager;

    int m_cameraMoveX;
    int m_cameraMoveY;
};
