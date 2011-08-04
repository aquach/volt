from pygame import *

import random
import threading
import time

time.sleep(1)

class State1(FSMState):
    def __init__(self):
        FSMState.__init__(self)

    def OnEnter (self):
        print 'entering state 1'
        self.TransitionTo('State2')

    def OnExit (self):
        print 'exiting state 1'


class State2(FSMState):
    def __init__(self):
        FSMState.__init__(self)

    def OnEnter (self):
        print 'entering state 2'


class MyEntity(PyEntity):
    def __init__(self):
        PyEntity.__init__(self)
        self.time = 0
        self.AddTag('MyEntity')
        self.fsm = FSM()
        self.fsm.AddStates([
            (State1(), "State1"),
            (State2(), "State2")
        ])
        self.fsm.TransitionTo("State1")

    def Update(self):
        self.time += dt()
        self.fsm.Update()

e = MyEntity()
scene().Add(e)

blendFilter = BlendFilter("Transition-BlendFilter", 50, -50)
blendFilter.thisown = 0
blendFilter.setBlendColor(Color.RGB(0, 0, 0))
scene().AddFilter(blendFilter)

def fadeOut():
    # TODO: Don't like the repeated time.sleeps. Maybe have a frame timer callback
    # that Python can subscribe to?
    duration = 2
    steps = 50
    start = time.time()
    for i in xrange(0, steps + 1):
        blendFilter.setBlendAmount(float(i) / steps)
        time.sleep(float(duration) / steps)
    print 'duration', time.time() - start
    level().RequestLevelChange("Levels/lights.json")
    blendFilter.setBlendAmount(0)
    
def onTouched(ladder, hit, contact):
    print 'fade'
    threading.Thread(target=fadeOut).start()

ladders = scene().GetAllTagged('Ladder')
for ladder in ladders:
    ladder.OnTouched(onTouched)

time.sleep(1)

while not level().IsUnloading():
    print 'hurr'
    time.sleep(1)

#PhysicsManager.Instance().SetGravity(Vector2(0, -30))

print 'Finished'
