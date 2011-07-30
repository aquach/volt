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

%rename(VoltEntity) Volt::Entity;
%include "../Volt/Game/Entity.h"
%include "Game/Entity.h"
%include "Entities/Game/Creature.h"
%include "Entities/Game/Light.h"
%include "Entities/Game/Player.h"
%include "Entities/Game/Triangle.h"
