import pygame
import pyvolt
import random
import time

triangles = []

for i in xrange(0, 5):
    a = pygame.EntityFactory.Create("Triangle")
    a.AddTag('barf' + str(i))
    print a.position()
    v = pyvolt.Vector2.Random() * 5
    a.SetPosition(v)
    pyvolt.Game.Instance().currentScene().Add(a)
    triangles.append(a)

time.sleep(4)

for tri in triangles:
    tri.SetScale(pygame.Vector2(4, 4))
