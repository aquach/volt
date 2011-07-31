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
#include "Volt/Assets/Asset.h"
#include "Volt/Assets/AssetRef.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Assets/ScriptAsset.h"

#include "Volt/Game/Entity.h"
#include "Volt/Game/Game.h"
#include "Volt/Graphics/Camera.h"
#include "Volt/Game/Scene.h"

%}

%include "std_string.i"
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
//%include "Volt/Assets/Asset.h"
//%include "Volt/Assets/AssetRef.h"
//%include "Volt/Assets/AssetManager.h"
//%include "Volt/Assets/ScriptAsset.h"

// String representation of common classes.
/*
%extend Volt::Vector2 {
    char* __str__ () {
        static char buffer[64];
        stringstream str;
        str << *$self;
        strcpy(buffer, str.str().c_str());
        return buffer;
    }
}
*/
