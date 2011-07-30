#include "Python.h"
#include <python2.7/Python.h>
#include "Volt/Assets/AssetManager.h"

namespace Volt {

namespace Python {

void Initialize () {
    Py_InitializeEx(0);
    PyEval_InitThreads();

    // TODO: Need to finalize bootstrap location and rectify Game/Volt divide.
    // Setup bootstrap correctly to import all modules.
    // Try to get some test scripts going.
    //PyRun_SimpleFile();
    PyRun_SimpleString(
        G_AssetManager->GetScript("../Game/Python/bootstrap.py")->script().c_str());
}

void Terminate () {
    Py_Finalize();
}

}

}
