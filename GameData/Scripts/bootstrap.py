import imp
import os
import threading
import time
import sys
import pyvoltbootstrap

# Add paths.
exeDir = os.path.dirname(os.path.abspath(sys.argv[0]))
sys.path.insert(0, exeDir)

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
        sys.modules[fullname] = mod
        mod.__file__ = fullname
        path = self.moduleNameToFilename(fullname)
        code = pyvoltbootstrap.getCode(path)
        exec code in mod.__dict__
        return mod

sys.meta_path.append(GameImporter())

import pyvolt
import pygame

v = pyvolt.Vector2(2, 3)
a = pyvolt.Vector2(4, 6)
