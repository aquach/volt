from pygame import *
from core.pygameutil import filenameToModuleName

import os
import re

"""Provides mechanisms to create and find entities implemented in Python."""

"""(ID, Class Name, Module Path). All three must be unique among all types.
ID must be greater than 0."""
ENTITY_TYPES = [
    (1, 'Turret', 'creatures.turret'),
    (2, 'MyEntity', 'test'),
]

ENTITY_TYPES_BY_ID = {}
for entityType in ENTITY_TYPES:
    ENTITY_TYPES_BY_ID[entityType[0]] = entityType
    
if len(ENTITY_TYPES_BY_ID) != len(ENTITY_TYPES):
    print 'Warning: ID collision in Python Entity types.'
    
ENTITY_TYPES_BY_NAME = {}
for entityType in ENTITY_TYPES:
    ENTITY_TYPES_BY_NAME[entityType[1]] = entityType

if len(ENTITY_TYPES_BY_NAME) != len(ENTITY_TYPES):
    print 'Warning: name collision in Python Entity types.'

def createEntity(entityTypeId):
    """A mechanism to instantiate arbitrary entities implemented in Python
    using dynamic import. For example, createEntity(1) where 1 is a
    turret.Turret will import turret and construct a Turret. createEntity
    returns the C++ object backing the Python entity. This is intended to be
    called from C++ so that Python entities can be instantiated in the level
    editor."""
    entityType = ENTITY_TYPES_BY_ID.get(entityTypeId, 0)
    if entityType == 0:
        raise Exception('Failed to find id %d' % entityTypeId)
    
    moduleName = entityType[2]
    className = entityType[1]
    module = __import__(moduleName, fromlist=[className])
    klass = getattr(module, className)
    instance = klass()
    instance.__disown__()
    return long(instance.this)

def findAllEntities(dirName):
    """Constructs a list of all Python entities and their module paths using
    regular expressions. UNUSED"""
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

