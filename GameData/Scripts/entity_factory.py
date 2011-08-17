from pygame import *

"""A mechanism to instantiate arbitrary entities implemented in Python using
dynamic import. For example, createEntity('turret.Turret') will import
turret and construct a Turret. createEntity returns the C++ object backing
the Python entity. This is intended to be called from C++ so that Python
entities can be instantiated in the level editor."""

def createEntity(entityPath):
    chunks = entityPath.rsplit('.', 1)
    if len(chunks) == 1:
        raise Exception("Invalid Python entity path '%s'" % entityPath)

    moduleName, className = chunks
    module = __import__(moduleName, fromlist=[className])
    klass = getattr(module, className)
    instance = klass()
    instance.__disown__()
    return long(instance.this)
