from pygame import *

import random
import threading
import time

import turret

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

@background
def moveCamera():
    player = scene().camera().watchingEntity()
    scene().camera().WatchEntity(None)

    duration = 1.5
    points = [scene().camera().position(), Vector2(3, 3), Vector2(5, -4),
              Vector2(10, 10), Vector2(-10, -4),
              scene().camera().position()]
    tween = CompositeTweenVector()
    for i in xrange(len(points) - 1):
        tween.AddTween(TweenVector.SinInOut(points[i], points[i + 1], duration))

    start = time.time()
    while not tween.finished():
        time.sleep(0.01)
        tween.SetTime(time.time() - start)
        scene().camera().transform.position = tween.value()

    tween = TweenFloat.SinInOut(0, 360, 4)
    start = time.time()
    while not tween.finished():
        time.sleep(0.01)
        tween.SetTime(time.time() - start)
        scene().camera().transform.rotation = tween.value()

    tween = CompositeTweenFloat()
    tween.AddTween(TweenFloat.SinInOut(30, 100, 3))
    tween.AddTween(TweenFloat.SinInOut(100, 30, 3))
    start = time.time()
    while not tween.finished():
        time.sleep(0.01)
        tween.SetTime(time.time() - start)
        scene().camera().transform.scale.Set(tween.value())

    scene().camera().WatchEntity(player)

def onTouched(ladder, hit, contact):
    print 'fade'
    #fadeOut()
    moveCamera()

ladders = scene().GetAllTagged('Ladder')
for ladder in ladders:
    ladder.OnTouched(onTouched)

@background
def platform():
    e = EntityFactory.Create('Triangle')
    e.SetScale(Vector2(3, 1))
    e.SetPosition(Vector2(3, -5))
    scene().Add(e)

    speed = 2
    duration = 3

    # Address issue with Triangles being static. Maybe have a setting for
    # bodies that have close-to-infinite mass but are dynamic? How is
    # mass going to work?
    tween = CompositeTweenFloat()
    tween.AddTween(TweenFloat.Linear(speed, -speed, duration))
    tween.AddTween(TweenFloat.Linear(-speed, speed, duration))
    start = time.time()
    while not level().IsUnloading():
        time.sleep(0.01)
        elapsed = time.time() - start
        if elapsed > duration * 2:
            start = time.time()
        tween.SetTime(elapsed)
        e.body().SetLinearVelocity(Vector2(0, tween.value()).ToB2())
        #print tween.value()
    e.RemoveSelf()
platform()

@background
def startConversation():
    messageBox("Hi, I'm a sign!")
    messageBox("""This is a really damn long message that spans a lot of lines. It goes on and on and is a lot of garbage nonsense. This is really awful and it goes on and won't stop for some reason.""")
    messageBox("""This is a long message that is
                line broken in Python.
                There's a break.""")
    messageBox("You cannot skip this!", canSkip=False)
    messageBox("You can't skip this one, but can move around!", canSkip=False, modal=False)
    messageBox("This is the last message (you can skip this but can't move)..", wait=True)
    print 'shown'

def onAccessed(sign, accessor):
    startConversation()

savePoints = scene().GetAllTagged('SavePoint')
for savePoint in savePoints:
    #GameEntity(sign).OnAccessed(onAccessed)
    addSavePoint(GameEntity(savePoint))

t = turret.Turret()
t.SetPosition(Vector2(5, -5))
scene().Add(t)


print 'before', Game.Instance().frameNumber()
time.sleep(0.0001)
print 'after', Game.Instance().frameNumber()

print 'before2', Game.Instance().frameNumber()
sleepFrame()
print 'after2', Game.Instance().frameNumber()

#PhysicsManager.Instance().SetGravity(Vector2(0, -30))

print 'Finished'
