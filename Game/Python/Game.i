%module pygame

%{
#include "Volt/Game/Entity.h"
#include "Game/Game/Entity.h"
#include "Game/Entities/Game/Creature.h"
#include "Game/Entities/Game/Light.h"
#include "Game/Entities/Game/Player.h"
#include "Game/Entities/Game/Triangle.h"

using Volt::Transform;

%}

%include "std_string.i"
%rename(VoltEntity) Volt::Entity;
%include "Volt/Game/Entity.h"
%include "Game/Game/Entity.h"
%include "Game/Entities/Game/Creature.h"
%include "Game/Entities/Game/Light.h"
%include "Game/Entities/Game/Player.h"
%include "Game/Entities/Game/Triangle.h"
