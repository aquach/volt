#include "Game/Python/PythonEntityFactory.h"
#include <Python.h>

namespace PythonEntityFactory {

Entity* CreateEntity(const string& path) {
    long result = 0;

    PyObject* moduleName = Py_BuildValue("s", "entity_factory");
    PyObject* module = PyImport_Import(moduleName);
    if (module) {
        PyObject* moduleDict = PyModule_GetDict(module);
        PyObject* func = PyDict_GetItemString(moduleDict, "createEntity");
        if (func != NULL && PyCallable_Check(func)) {
            PyObject* args = Py_BuildValue("(s)", path.c_str());
            PyObject* returnValue = PyObject_CallObject(func, args);
            if (returnValue) {
                result = PyLong_AsLong(returnValue);
                Py_XDECREF(returnValue);
            }
            Py_XDECREF(args);
        }
        Py_XDECREF(module);
    }
    Py_XDECREF(moduleName);

    return (Entity*)result;
}

void GetEntityTypes (const string& searchPath, vector<string>* entityNames,
                     vector<string>* entityPaths) {
    PyObject* moduleName = Py_BuildValue("s", "entity_factory");
    PyObject* module = PyImport_Import(moduleName);
    if (module) {
        PyObject* moduleDict = PyModule_GetDict(module);
        PyObject* func = PyDict_GetItemString(moduleDict, "findAllEntities");
        if (func != NULL && PyCallable_Check(func)) {
            PyObject* args = Py_BuildValue("(s)", searchPath.c_str());
            PyObject* returnValue = PyObject_CallObject(func, args);
            if (returnValue) {
                CHECK(PyList_Check(returnValue));
                int size = PyList_Size(returnValue);
                for (int i = 0; i < size; i++) {
                    PyObject* tuple = PyList_GetItem(returnValue, i);
                    const char* entityName = NULL;
                    const char* entityPath = NULL;
                    PyArg_ParseTuple(tuple, "ss", &entityName, &entityPath);
                    entityNames->push_back(entityName);
                    entityPaths->push_back(entityPath);
                }
                Py_XDECREF(returnValue);
            }
            Py_XDECREF(args);
        }
        Py_XDECREF(module);
    }
    Py_XDECREF(moduleName);
}

}
