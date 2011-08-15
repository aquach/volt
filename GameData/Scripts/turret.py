from pygame import *

import time
import targeting

class Turret(PyEntity):
    class TurretProjectile(PyEntity):
        SPEED = 2
        def __init__(self):
            PyEntity.__init__(self)
            self.AddTag('TurretProjectile')

        def OnAdded(self):
            # Create body.
            pass

        def Update(self):
            pass
        
    def __init__(self):
        PyEntity.__init__(self)
        self.AddTag('Turret')
        self.lastFiredProjectile = 0

    def OnAdded(self):
        # Create body.
        pass
        
    def Update(self):
        player = scene().GetFirstTagged('Player')
        if time.time() - self.lastFiredProjectile > 3:
            self.lastFiredProjectile = time.time()
            playerVel = Vector2(player.body().GetLinearVelocity())
            t, hitPos = targeting.linearTarget(player.position(),
                                               playerVel,
                                               self.position(),
                                               Turret.TurretProjectile.SPEED)
            direction = hitPos - self.position()
            direction.Normalize()
            p = Turret.TurretProjectile()
            p.SetPosition(self.position())
            p.body().SetLinearVelocity((direction * Turret.TurretProjectile.SPEED).ToB2())
            scene().Add(p)
            
            
                                   
