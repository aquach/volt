from pygame import *

import time
import targeting

class Turret(PyEntity):
    class TurretProjectile(PyEntity):
        SPEED = 8
        TIMEOUT = 10
        def __init__(self, startPos, hitPos):
            PyEntity.__init__(self)
            self.AddTag('TurretProjectile')
            self.SetPosition(startPos)
            self.dir = hitPos - startPos
            self.dir.Normalize()
            self.startTime = time.time()
            self.OnTouched(self.OnHit)

        def OnHit(self, self2, hitEntity, contact):
            if hitEntity.HasTag('Player'):
                print 'hit player!'
            elif hitEntity.HasTag('Triangle'):
                self.RemoveSelf()
            print hitEntity

        def OnAdded(self):
            self.CreateBody('box', sensor=True)
            self.body().SetLinearVelocity((self.dir * Turret.TurretProjectile.SPEED).ToB2())

        def Update(self):
            self.UpdatePhysics()
            if time.time() - self.startTime > Turret.TurretProjectile.TIMEOUT:
                self.RemoveSelf()
                return

    def __init__(self):
        PyEntity.__init__(self)
        self.lastFiredProjectile = 0

    def OnAdded(self):
        self.CreateBody('box', dynamic=False)

    def Update(self):
        if not gameMode(): return
        player = scene().GetFirstTagged('Player')
        if time.time() - self.lastFiredProjectile > 3:
            self.lastFiredProjectile = time.time()
            playerVel = Vector2(player.body().GetLinearVelocity())
            hitData = targeting.linearTarget(player.position(),
                                             playerVel,
                                             self.position(),
                                             Turret.TurretProjectile.SPEED)
            if hitData:
                t, hitPos = hitData
            else:
                hitPos = player.position()

            direction = hitPos - self.position()
            direction.Normalize()
            p = Turret.TurretProjectile(self.position(), hitPos)
            scene().Add(p)



