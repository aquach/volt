#include "Game/Python/PythonEntityFactory.h"
#include <Python.h>

namespace PythonEntityFactory {

Entity* CreateEntity (int id) {
    long result = 0;

    PyObject* moduleName = Py_BuildValue("s", "entity_factory");
    PyObject* module = PyImport_Import(moduleName);
    if (module) {
        PyObject* moduleDict = PyModule_GetDict(module);
        PyObject* func = PyDict_GetItemString(moduleDict, "createEntity");
        if (func != NULL && PyCallable_Check(func)) {
            PyObject* args = Py_BuildValue("(i)", id);
            PyObject* returnValue = PyObject_CallObject(func, args);
            if (returnValue) {
                result = PyLong_AsLong(returnValue);
                Py_XDECREF(returnValue);
            } else {
                LOG(WARNING) << "No return value for "
                             << "entity_factory.createEntity";
            }
            Py_XDECREF(args);
        } else {
            LOG(WARNING) << "Could not find createEntity call.";
        }
        Py_XDECREF(module);
    } else {
        LOG(WARNING) << "Could not find module entity_factory.";
    }
    Py_XDECREF(moduleName);

    return (Entity*)result;
}

void GetEntityTypes (const string& searchPath, vector<string>* entityNames,
                     vector<int>* entityIds) {
    PyObject* moduleName = Py_BuildValue("s", "entity_factory");
    PyObject* module = PyImport_Import(moduleName);
    if (module) {
        PyObject* moduleDict = PyModule_GetDict(module);
        PyObject* list = PyDict_GetItemString(moduleDict, "ENTITY_TYPES");
        CHECK(PyList_Check(list));
        int size = PyList_Size(list);
        for (int i = 0; i < size; i++) {
            PyObject* tuple = PyList_GetItem(list, i);
            int id = 0;
            const char* entityName = NULL;
            const char* entityPath = NULL;
            PyArg_ParseTuple(tuple, "iss", &id, &entityName, &entityPath);
            entityNames->push_back(entityName);
            entityIds->push_back(id);
        }
        Py_XDECREF(list);
        Py_XDECREF(module);
    }
    Py_XDECREF(moduleName);
}

}
