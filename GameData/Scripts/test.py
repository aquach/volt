from pygame import *

import random
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

def onTouched(ladder, hit, contact):
    level().RequestLevelChange("Levels/lights.json")

ladder = scene().GetAllTagged('Ladder')[1]
ladder.OnTouched(onTouched)

time.sleep(1)

while not level().IsUnloading():
    print 'hurr'
    time.sleep(1)

#PhysicsManager.Instance().SetGravity(Vector2(0, -30))

print 'Finished'
