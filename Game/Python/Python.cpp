#include "Game/Python/Game_wrap.cxx"
#include "Volt/Assets/AssetManager.h"

namespace Python {

void Initialize () {
    // Import Python bindings into main Python context directly.
    SWIG_init();
    PyRun_SimpleString(
        G_AssetManager->GetScript("Scripts/bootstrap.py")->script().c_str());
}

}
