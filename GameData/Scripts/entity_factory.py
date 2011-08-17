from pygame import *
from pygameutil import filenameToModuleName

import os
import re

"""Provides mechanisms to create and find entities implemented in Python."""

def createEntity(entityPath):
    """A mechanism to instantiate arbitrary entities implemented in Python
    using dynamic import. For example, createEntity('turret.Turret') will
    import turret and construct a Turret. createEntity returns the C++
    object backing the Python entity. This is intended to be called from C++
    so that Python entities can be instantiated in the level editor."""
    chunks = entityPath.rsplit('.', 1)
    if len(chunks) == 1:
        raise Exception("Invalid Python entity path '%s'" % entityPath)

    moduleName, className = chunks
    module = __import__(moduleName, fromlist=[className])
    klass = getattr(module, className)
    instance = klass()
    instance.__disown__()
    return long(instance.this)

def findAllEntities(dirName):
    entities = []
    for root, dirs, files in os.walk(dirName):
        for filename in files:
            if filename.endswith('.py'):
                data = open(os.path.join(root, filename)).read()
                groups = re.findall(r'^class (\w+)\(PyEntity\)', data,
                                    re.MULTILINE)
                relativePath = os.path.join(root, filename)[len(dirName) + 1:]
                moduleName = filenameToModuleName(relativePath)
                for group in groups:
                    entities.append((group, moduleName + '.' + group))
    return entities

