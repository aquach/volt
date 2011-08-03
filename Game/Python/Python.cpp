#include "Python.h"
#include <Python.h>
#include "Volt/Assets/AssetManager.h"

namespace Python {

static PyObject* getCode (PyObject* self, PyObject* args) {
    char* path;
    if (!PyArg_ParseTuple(args, "s", &path))
        return Py_BuildValue("");

    if (!G_AssetManager->assetExists(path))
        return Py_BuildValue("");

    return Py_BuildValue(
        "s",
        G_AssetManager->GetScript(path)->script().c_str());
}

static PyMethodDef PyVoltBootstrapMethods[] = {
    {"getCode", getCode, METH_VARARGS,
     "Returns the code of the requested script."},
    {NULL, NULL, 0, NULL}
};

void Initialize (int argc, char** argv) {
    Py_InitializeEx(0);
    PySys_SetArgvEx(argc, argv, 0);
    PyEval_InitThreads();
    Py_InitModule("pyvoltbootstrap", PyVoltBootstrapMethods);

    PyRun_SimpleString(
        G_AssetManager->GetScript("Scripts/bootstrap.py")->script().c_str());
}

void Terminate () {
    Py_Finalize();
}

void RunGameScriptFile (const string& scriptPath) {
    // Hack!
    char command[128];
    sprintf(command, "pygameutil.runScriptFile('%s')", scriptPath.c_str());
    PyRun_SimpleString(command);
}

}
