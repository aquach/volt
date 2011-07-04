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

    //virtual void OnKeyEvent (SDL_KeyboardEvent event);
    //virtual void OnMouseButtonEvent (SDL_MouseButtonEvent event);
    //virtual void OnMouseMoveEvent (SDL_MouseMotionEvent event);

private:
    LevelManager* m_levelManager;
};
