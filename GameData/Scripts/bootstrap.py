import imp
import os
import sys
import pyvoltbootstrap

"""Initialization module that sets up proper importing of modules with a custom
handler."""

class GameImporter(object):
    """Custom importer that calls into AssetManager."""
    def moduleNameToFilename(self, moduleName):
        moduleName = moduleName.replace('.', '/')
        return 'Scripts/' + moduleName + '.py'

    def packageNameToFilename(self, packageName):
        packageName = packageName.replace('.', '/')
        return 'Scripts/' + packageName + '/__init__.py'


    def getCode(self, fullname):
        # Choose between package and module loading.
        path = self.moduleNameToFilename(fullname)
        packagePath = self.packageNameToFilename(fullname)
        code = pyvoltbootstrap.getCode(path)
        packageCode = pyvoltbootstrap.getCode(packagePath)
        
        path = packagePath if packagePath else path
        code = packageCode if packageCode is not None else code
        return path, code
                
    def find_module(self, fullname, path=None):
        
        path, code = self.getCode(fullname)
        if code is not None:
            return self
        else:
            return None

    def load_module(self, fullname):
        if fullname in sys.modules:
            return sys.modules[fullname]
        mod = imp.new_module(fullname)
        mod.__loader__ = self
        mod.__name__ = fullname
        sys.modules[fullname] = mod
        path, code = self.getCode(fullname)
        mod.__file__ = path
        if code:
            exec code in mod.__dict__
        else:
            # Package
            mod.__path__ = fullname
        return mod

sys.meta_path.append(GameImporter())

import pygame
import core.pygameutil
