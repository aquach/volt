%module(directors="1") pygame;

%{

#include <Box2D/Box2D.h>
#include "Volt/Core/BBox.h"
#include "Volt/Core/Color.h"
#include "Volt/Core/Math.h"
#include "Volt/Core/Random.h"
//#include "Volt/Core/System.h"
#include "Volt/Core/Time.h"
#include "Volt/Core/Transform.h"
#include "Volt/Core/Tween.h"
#include "Volt/Core/Vector2.h"

#include "Volt/Game/AppTime.h"
#include "Volt/Game/Entity.h"
#include "Volt/Game/FSM.h"
#include "Volt/Game/Game.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/Camera.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/Filter.h"
#include "Volt/Graphics/SpriteAnimation.h"
#include "Volt/Game/Scene.h"

#include "Game/Filters/BlendFilter.h"
#include "Game/Filters/EdgeFilter.h"
#include "Game/Game/Entity.h"
#include "Game/Game/LevelManager.h"
#include "Game/Game/SaveData.h"
#include "Game/Entities/Game/Creature.h"
#include "Game/Entities/Game/Light.h"
#include "Game/Entities/Game/Triangle.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Entities/Gui/ChoiceBox.h"
#include "Game/Entities/Gui/DialogBox.h"
#include "Game/Entities/Gui/MessageBox.h"
#include "Game/Scenes/GameScene.h"

%}

// SWIG libs.
%include <std_string.i>
%include <std_vector.i>

namespace std {
    %template(EntityVector) vector<Volt::Entity*>;
    %template(StringVector) vector<std::string>;
}

// Print errors when something bad happens.
%feature("director:except") {
    if ($error != NULL) {
        PyErr_PrintEx(0);
    }
}

// Global ignores.
%ignore DECLARE_ENTITY_;
%ignore *::operator=;
%ignore *::operator[];
%ignore operator<<;
%ignore operator+;
%ignore operator-;
%ignore operator*;
%ignore operator==;

// Box2D libs.
%include <Box2D/Common/b2Math.h>
%include <Box2D/Dynamics/b2Body.h>
//%include <Box2D/Dynamics/b2Fixture.h>
//%include <Box2D/Collision/Shapes/b2Shape.h>
//%include <Box2D/Collision/Shapes/b2CircleShape.h>
//%include <Box2D/Collision/Shapes/b2PolygonShape.h>
//%include <Box2D/Collision/Shapes/b2PolygonShape.h>

// SDL Input.
/** Keyboard event structure */
typedef struct SDL_KeyboardEvent {
	Uint8 type;	/**< SDL_KEYDOWN or SDL_KEYUP */
	Uint8 which;	/**< The keyboard device index */
	Uint8 state;	/**< SDL_PRESSED or SDL_RELEASED */
	SDL_keysym keysym;
} SDL_KeyboardEvent;
%include <SDL/SDL_keysym.h>

// Volt.
%include "Volt/Core/Color.h"
%include "Volt/Core/Math.h"
%include "Volt/Core/Random.h"
//%include "Volt/Core/System.h"
%include "Volt/Core/Time.h"
%include "Volt/Core/Vector2.h"
%include "Volt/Core/Transform.h"

%include "Volt/Core/Tween.h"
%template(TweenVector) Volt::Tween<Volt::Vector2>;
%template(TweenFloat) Volt::Tween<float>;
%template(CompositeTweenVector) Volt::CompositeTween<Volt::Vector2>;
%template(CompositeTweenFloat) Volt::CompositeTween<float>;

%include "Volt/Core/BBox.h"
%include "Volt/Core/Vector3.h"

%include "Volt/Graphics/Camera.h"
%include "Volt/Graphics/Filter.h"
%ignore Volt::SpriteAnimation::SpriteAnimation(Entity*, DataAssetRef);
%include "Volt/Graphics/SpriteAnimation.h"

%include "Volt/Game/AppTime.h"

%rename(VoltEntity) Volt::Entity;
%rename(CreateBodyWithDef) Volt::Entity::CreateBody(b2BodyDef);
%rename(CreateBodyWithType) Volt::Entity::CreateBody(b2BodyType);
%feature("director") EntityContactListener;
%include "Volt/Game/Entity.h"

%feature("director") FSMState;
%pythonappend Volt::FSMState::FSMState() %{
   self.__disown__()
%}
%include "Volt/Game/FSM.h"

%include "Volt/Game/Game.h"
%include "Volt/Game/PhysicsManager.h"

%feature("shadow") Volt::Scene::GetAllTagged(const string&, vector<Entity*>*) %{
    def GetAllTagged(self, tag):
        entities = EntityVector()
        _pygame.Scene_GetAllTagged(self, tag, entities)
        return entities
%}
%include "Volt/Game/Scene.h"

// Game.

%include "Game/Filters/BlendFilter.h"
%include "Game/Filters/EdgeFilter.h"

%feature("director") Entity;
%feature("director") EntityAccessListener;
%feature("shadow") Entity::OnAccessed(Entity*) %{
    def OnAccessed(self, callback):
        pylistener = PyEntityAccessListener(self, callback)
        pylistener.__disown__()
        self.AddAccessListener(pylistener)
        return pylistener
%}
%include "Game/Game/Entity.h"
%include "Game/Game/LevelManager.h"
%include "Game/Game/SaveData.h"
%feature("director") Creature;
%include "Game/Entities/Game/Creature.h"

%ignore Light::STATIC_MAP_FOLDER;
%include "Game/Entities/Game/Light.h"
//%include "Game/Entities/Game/Player.h"
%include "Game/Entities/Game/Triangle.h"
%include "Game/Editor/EntityFactory.h"
%feature("director") DialogListener;
%include "Game/Entities/Gui/DialogBox.h"
%include "Game/Entities/Gui/ChoiceBox.h"
%include "Game/Entities/Gui/MessageBox.h"
%include "Game/Scenes/GameScene.h"

// Importing custom python code.
%pythoncode {
    from core.pygamecore import *
}

// Casting helper methods.
%inline %{

Entity* GameEntity (Volt::Entity* e) {
   return dynamic_cast<Entity*>(e);
}

Creature* CreatureCast (Volt::Entity* e) {
   return dynamic_cast<Creature*>(e);
}

GameScene* scene () {
    return dynamic_cast<GameScene*>(Volt::Game::Instance()->currentScene());
}

%}

// Extending classes for easier functionality.

%extend MessageBox {
    %pythoncode {
        def WaitForFinish(self):
            class DialogFinishListener(DialogListener):
                def __init__(self, alertSemaphore):
                    DialogListener.__init__(self)
                    self.__disown__()
                    self.alertSemaphore = alertSemaphore

                def OnDialogFinished(self):
                    self.alertSemaphore.release()

            semaphore = threading.Semaphore(0)
            listener = DialogFinishListener(semaphore)
            self.AddDialogListener(listener)

            semaphore.acquire()
    }
}

%extend ChoiceBox {
    %pythoncode {
        def WaitForChoice(self):
            class ChoiceFinishListener(DialogListener):
                def __init__(self, choiceBox, alertSemaphore):
                    DialogListener.__init__(self)
                    self.__disown__()
                    self.choiceBox = choiceBox
                    self.choice = -1
                    self.alertSemaphore = alertSemaphore

                def OnDialogFinished(self):
                    self.choice = self.choiceBox.choice()
                    self.alertSemaphore.release()

            semaphore = threading.Semaphore(0)
            listener = ChoiceFinishListener(self, semaphore)
            self.AddDialogListener(listener)

            semaphore.acquire()

            return listener.choice
    }
}

%extend Volt::FSM {
    %pythoncode {
        def AddStates(self, stateList):
            for state in stateList:
                self.AddState(state[0], state[1])
    }
}

%extend Volt::Entity {
    char* __str__ () {
        static char buffer[64];
        stringstream str;
        str << *$self;
        strcpy(buffer, str.str().c_str());
        return buffer;
    }

    %pythoncode {
        def OnTouched(self, callback):
            pylistener = PyEntityContactListener(self, callback, None)
            pylistener.__disown__()
            self.AddContactListener(pylistener)
            return pylistener

        def CreateBody(self, bodyType, dynamic=True, ignoresForces=False,
                       sensor=False):
            self.CreateBodyCustom(bodyType, dynamic, ignoresForces, sensor)
    }
}

%extend Volt::Vector2 {
    char* __str__ () {
        static char buffer[64];
        stringstream str;
        str << *$self;
        strcpy(buffer, str.str().c_str());
        return buffer;
    }
}
