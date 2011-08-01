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

tweens = []
for tri in triangles:
    tweens.append(TweenVector.Linear(tri.position(), Vector2(3, 3), 4))

while True:
    for i, tri in enumerate(triangles):
        tweens[i].Update(0.01)
        tri.SetPosition(tweens[i].value())
    time.sleep(0.01)
