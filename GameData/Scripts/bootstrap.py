import imp
import os
import sys
import pyvoltbootstrap

# Add path for shared libraries.
exeDir = os.path.dirname(os.path.abspath(sys.argv[0]))
sys.path.insert(0, exeDir)

# Custom importer that calls into AssetManager.
class GameImporter(object):
    def moduleNameToFilename(self, moduleName):
        return 'Scripts/' + moduleName + '.py'

    def find_module(self, fullname, path=None):
        path = self.moduleNameToFilename(fullname)
        code = pyvoltbootstrap.getCode(path)
        if code:
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
        path = self.moduleNameToFilename(fullname)
        mod.__file__ = path
        code = pyvoltbootstrap.getCode(path)
        exec code in mod.__dict__
        return mod

sys.meta_path.append(GameImporter())

import pygame
import pygameutil
