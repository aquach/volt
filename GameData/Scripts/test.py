from pygame import *
from pygamecore import *

import random
import time

triangles = []

for i in xrange(0, 5):
    a = EntityFactory.Create('Triangle')
    a.AddTag('barf' + str(i))
    a.AddTag('fart')
    v = Vector2.Random() * 5
    a.SetPosition(v)
    scene().Add(a)
    a.RemoveTag('fart')
    triangles.append(a)

entities = scene().GetAllTagged('Sign')

while True:
    for tri in triangles:
        tri.SetPosition(tri.position() + Vector2(0.1, 0.1))
    time.sleep(0.05)
