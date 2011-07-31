import random

for i in xrange(0, 5):
    a = pygame.Triangle()
    a.AddTag('barf' + str(i))
    print a.position()
    v = pyvolt.Vector2.Random() * 5
    a.SetPosition(v)
    pyvolt.Game.Instance().currentScene().Add(a)
    
