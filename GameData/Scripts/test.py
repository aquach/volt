from pygame import *
from pygamecore import *

import random
import time

class MyEntity(PyEntity):
    def __init__(self):
        PyEntity.__init__(self)
        self.time = 0
        self.AddTag('MyEntity')

    def Update(self):
        self.time += dt()
        self.a += 4
        print 'update ' + str(self.time)

e = MyEntity()
scene().Add(e)
