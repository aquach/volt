#include "Python.h"
#include <python2.7/Python.h>
#include "Volt/Assets/AssetManager.h"

namespace Volt {

namespace Python {

void Initialize () {
    Py_InitializeEx(0);
    PyEval_InitThreads();

    PyRun_SimpleString(
        G_AssetManager->GetScript("bootstrap.py")->script().c_str());
}

void Terminate () {
    Py_Finalize();
}

}

}
