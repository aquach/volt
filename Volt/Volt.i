%module pyvolt

%{
#include "Volt/Core/BBox.h"
#include "Volt/Core/Color.h"
#include "Volt/Core/Math.h"
#include "Volt/Core/Random.h"
#include "Volt/Core/System.h"
#include "Volt/Core/Time.h"
#include "Volt/Core/Transform.h"
#include "Volt/Core/Vector2.h"
#include "Volt/Core/Vector3.h"

#include "Volt/Game/Entity.h"

using namespace Volt;

%}

%include "Core/BBox.h"
%include "Core/Color.h"
%include "Core/Math.h"
%include "Core/Random.h"
%include "Core/System.h"
%include "Core/Time.h"
%include "Core/Transform.h"
%include "Core/Vector2.h"
%include "Core/Vector3.h"
%include "Game/Entity.h"
