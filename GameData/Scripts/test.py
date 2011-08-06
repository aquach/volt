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

#e = MyEntity()
#scene().Add(e)

blendFilter = disown(BlendFilter("Transition-BlendFilter", 50, -50))
blendFilter.setBlendColor(Color.RGB(0, 0, 0))
scene().AddFilter(blendFilter)

@background
def fadeOut():
    tween = TweenFloat.Linear(0, 1, 2)
    start = time.time()
    while not tween.finished():
        time.sleep(0.01)
        tween.SetTime(time.time() - start)
        blendFilter.setBlendAmount(tween.value())
    level().RequestLevelChange("Levels/lights.json")
    time.sleep(2)
    tween = TweenFloat.Linear(1, 0, 2)
    start = time.time()
    # TODO: The main script executed by runScriptFile is joined when the level
    # unloads. But any threads that it spawns will hang around. This might be a
    # problem.
    while not tween.finished():
        time.sleep(0.01)
        tween.SetTime(time.time() - start)
        blendFilter.setBlendAmount(tween.value())

def onTouched(ladder, hit, contact):
    print 'fade'
    #fadeOut()

ladders = scene().GetAllTagged('Ladder')
for ladder in ladders:
    ladder.OnTouched(onTouched)

@background
def platform():
    e = EntityFactory.Create('Triangle')
    e.SetScale(Vector2(3, 1))
    scene().Add(e)

    start = Vector2(3, -5)
    end = start + Vector2(0, -5)
    duration = 3

    tween = CompositeTweenVector();
    tween.AddTween(TweenVector.Linear(start, end, duration))
    tween.AddTween(TweenVector.QuadraticIn(end, start, duration))
    start = time.time()
    while not level().IsUnloading():
        time.sleep(0.01)
        elapsed = time.time() - start
        if elapsed > duration * 2:
            start = time.time()
        tween.SetTime(elapsed)
        e.SetPosition(tween.value())
    e.RemoveSelf()
platform()

def onAccessed(sign, accessor):
    print 'hi im a sign', accessor  
    
signs = scene().GetAllTagged('Sign')
for sign in signs:
    GameEntity(sign).OnAccessed(onAccessed)

print 'before', Game.Instance().frameNumber()
time.sleep(0.0001)
print 'after', Game.Instance().frameNumber()

print 'before2', Game.Instance().frameNumber()
sleepFrame()
print 'after2', Game.Instance().frameNumber()

#PhysicsManager.Instance().SetGravity(Vector2(0, -30))

print 'Finished'
