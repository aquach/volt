%module pygame

%{
#include "Volt/Core/BBox.h"
#include "Volt/Core/Color.h"
#include "Volt/Core/Math.h"
#include "Volt/Core/Random.h"
#include "Volt/Core/System.h"
#include "Volt/Core/Time.h"
#include "Volt/Core/Transform.h"
#include "Volt/Core/Vector2.h"

#include "Volt/Game/Entity.h"
#include "Volt/Game/Game.h"
#include "Volt/Graphics/Camera.h"
#include "Volt/Game/Scene.h"

#include "Game/Game/Entity.h"
#include "Game/Entities/Game/Creature.h"
#include "Game/Entities/Game/Light.h"
#include "Game/Entities/Game/Player.h"
#include "Game/Entities/Game/Triangle.h"
#include "Game/Editor/EntityFactory.h"

%}

%ignore DECLARE_ENTITY_;
%rename(VoltEntity) Volt::Entity;

%include <std_string.i>
%include <std_vector.i>

%include "Volt/Core/Color.h"
%include "Volt/Core/Math.h"
%include "Volt/Core/Random.h"
%include "Volt/Core/System.h"
%include "Volt/Core/Time.h"
%include "Volt/Core/Vector2.h"
%include "Volt/Core/Transform.h"
%include "Volt/Core/BBox.h"
%include "Volt/Core/Vector3.h"
%include "Volt/Game/Entity.h"
%include "Volt/Game/Game.h"
%include "Volt/Graphics/Camera.h"
%include "Volt/Game/Scene.h"

%include "Game/Game/Entity.h"
%include "Game/Entities/Game/Creature.h"
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
