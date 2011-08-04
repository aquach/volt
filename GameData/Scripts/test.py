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

blendFilter = disown(BlendFilter("Transition-BlendFilter", 50, -50))
blendFilter.setBlendColor(Color.RGB(0, 0, 0))
scene().AddFilter(blendFilter)

@background
def fadeOut():
    # TODO: sleepFrame call that sleeps for exactly one frame.
    duration = 2
    steps = 30
    start = time.time()
    timePerStep = float(duration) / steps
    sleeps = 0
    for i in xrange(0, steps):
        time.sleep(timePerStep)
        blendFilter.setBlendAmount(float(i) / steps)
        sleeps += 1
    print 'duration', time.time() - start
    print sleeps, 'sleeps'
    level().RequestLevelChange("Levels/lights.json")
    blendFilter.setBlendAmount(0)
    
def onTouched(ladder, hit, contact):
    print 'fade'
    fadeOut()

ladders = scene().GetAllTagged('Ladder')
for ladder in ladders:
    ladder.OnTouched(onTouched)

time.sleep(1)

while not level().IsUnloading():
    print 'hurr'
    time.sleep(1)

#PhysicsManager.Instance().SetGravity(Vector2(0, -30))

print 'Finished'
