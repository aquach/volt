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
#include "Volt/Game/Game.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Graphics/Camera.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/Filter.h"
#include "Volt/Game/Scene.h"

#include "Game/Filters/BlendFilter.h"
#include "Game/Filters/EdgeFilter.h"
#include "Game/Game/Entity.h"
#include "Game/Game/LevelManager.h"
#include "Game/Game/SaveData.h"
#include "Game/Entities/Game/Creature.h"
#include "Game/Entities/Game/Light.h"
#include "Game/Entities/Game/Player.h"
#include "Game/Entities/Game/Triangle.h"
#include "Game/Editor/EntityFactory.h"
#include "Game/Entities/Gui/ChoiceBox.h"
#include "Game/Entities/Gui/DialogBox.h"
#include "Game/Entities/Gui/MessageBox.h"
#include "Game/Scenes/GameScene.h"

%}

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

// SWIG libs.
%include <std_string.i>
%include <std_vector.i>

// Box2D libs.
%include <Box2D/Common/b2Math.h>
%include <Box2D/Dynamics/b2Body.h>

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

%include "Volt/Game/AppTime.h"

%rename(VoltEntity) Volt::Entity;
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
%include "Game/Entities/Game/Creature.h"

%ignore Light::STATIC_MAP_FOLDER;
%include "Game/Entities/Game/Light.h"
%include "Game/Entities/Game/Player.h"
%include "Game/Entities/Game/Triangle.h"
%include "Game/Editor/EntityFactory.h"
%include "Game/Entities/Gui/DialogBox.h"
//%include "Game/Entities/Gui/ChoiceBox.h"
%include "Game/Entities/Gui/MessageBox.h"
%include "Game/Scenes/GameScene.h"

namespace std {
    %template(EntityVector) vector<Volt::Entity*>;
}

// Importing custom python code.
%pythoncode {
    from pygamecore import *
}

// Casting helper methods.
%inline %{

Entity* GameEntity (Volt::Entity* e) {
   return dynamic_cast<Entity*>(e);
}

GameScene* scene () {
    return dynamic_cast<GameScene*>(Volt::Game::Instance()->currentScene());
}

%}

// Extending classes for easier functionality.

%extend MessageBox {
    %pythoncode {
        def WaitForFinish(self):
            while not self.IsFinished():
                time.sleep(0.25)
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
