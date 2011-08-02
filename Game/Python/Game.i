%module(directors="1") pygame;

%{

#include <Box2D/Box2D.h>
#include "Volt/Core/BBox.h"
#include "Volt/Core/Color.h"
#include "Volt/Core/Math.h"
#include "Volt/Core/Random.h"
#include "Volt/Core/System.h"
#include "Volt/Core/Time.h"
#include "Volt/Core/Transform.h"
#include "Volt/Core/Tween.h"
#include "Volt/Core/Vector2.h"

#include "Volt/Game/AppTime.h"
#include "Volt/Game/Entity.h"
#include "Volt/Game/Game.h"
#include "Volt/Graphics/Camera.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Game/Scene.h"

#include "Game/Game/Entity.h"
#include "Game/Entities/Game/Creature.h"
#include "Game/Entities/Game/Light.h"
#include "Game/Entities/Game/Player.h"
#include "Game/Entities/Game/Triangle.h"
#include "Game/Editor/EntityFactory.h"

%}

// Print errors when something bad happens.
%feature("director:except") {
    if ($error != NULL) {
        PyErr_PrintEx(0);
    }
}

%ignore DECLARE_ENTITY_;
%rename(VoltEntity) Volt::Entity;

%include <std_string.i>
%include <std_vector.i>

%include <Box2D/Common/b2Math.h>
%include <Box2D/Dynamics/b2Body.h>

%include "Volt/Core/Color.h"
%include "Volt/Core/Math.h"
%include "Volt/Core/Random.h"
%include "Volt/Core/System.h"
%include "Volt/Core/Time.h"
%include "Volt/Core/Vector2.h"
%include "Volt/Core/Transform.h"

%include "Volt/Core/Tween.h"
%template(TweenVector) Volt::Tween<Volt::Vector2>;
%template(TweenFloat) Volt::Tween<float>;

%include "Volt/Core/BBox.h"
%include "Volt/Core/Vector3.h"
%include "Volt/Game/AppTime.h"
%include "Volt/Game/Entity.h"
%include "Volt/Game/Game.h"
%include "Volt/Graphics/Camera.h"
//%include "Volt/Graphics/Graphics.h"
%include "Volt/Game/Scene.h"

%feature("director") Entity;
%include "Game/Game/Entity.h"
%include "Game/Entities/Game/Creature.h"

%ignore Light::STATIC_MAP_FOLDER;
%include "Game/Entities/Game/Light.h"
%include "Game/Entities/Game/Player.h"
%include "Game/Entities/Game/Triangle.h"
%include "Game/Editor/EntityFactory.h"

namespace std {
    %template(EntityVector) vector<Volt::Entity*>;
}

// String representation of common classes.
%extend Volt::Vector2 {
    char* __str__ () {
        static char buffer[64];
        stringstream str;
        str << *$self;
        strcpy(buffer, str.str().c_str());
        return buffer;
    }
}

%extend Volt::Entity {
    void SetVelocity (Vector2 velocity) {
        $self->body()->SetLinearVelocity(velocity.ToB2());
    }
}
