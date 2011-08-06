import pygame
import threading
import time

'''Handy method to get scene pointer.'''
def scene():
    return pygame.Game.Instance().currentScene()

def dt():
    return pygame.AppTime.Instance().dt()

def level():
    return pygame.LevelManager.Instance()

def disown(swigObj):
    swigObj.thisown = 0
    return swigObj

''' Sleep, guarding against the level unloading while sleeping.'''
def longSleep(duration):
    while not level().IsUnloading() and duration > 0:
        sleepTime = min(duration, 1)
        duration -= sleepTime
        time.sleep(sleepTime)

'''Sleep until the frame counter has changed, indicating a frame has passed.'''
def sleepFrame():
    currentFrame = pygame.Game.Instance().frameNumber()
    while currentFrame == pygame.Game.Instance().frameNumber():
        time.sleep(0.01)


def background(f):
    """
    a threading decorator
    use @background above the function you want to thread
    (run in the background)
    """
    def bg_f(*a, **kw):
        thread = threading.Thread(name=f.__name__, target=f, args=a, kwargs=kw)
        thread.start()
        return thread
    return bg_f

class PyEntity(pygame.Entity):
    ''' Wrapper class for defining your own entities.'''
    def __init__(self, disown=True):
        pygame.Entity.__init__(self)
        # Disown pointer from Python because we're probably going to give
        # it to the Scene to handle.
        if disown:
            self.__disown__()

    def OnAdded(self):
        pass

    def OnRemoved(self):
        pass

    def Update(self):
        pass

    def Render(self):
        pass

class PyEntityContactListener(pygame.EntityContactListener):
    def __init__(self, entity, beginCallback, endCallback):
        pygame.EntityContactListener.__init__(self)
        self.entity = entity
        self.beginCallback = beginCallback
        self.endCallback = endCallback

    def OnContactBegin(self, other, contact):
        if self.beginCallback:
            self.beginCallback(self.entity, other, contact)

    def OnContactEnd(self, other, contact):
        if self.endCallback:
            self.endCallback(self.entity, other, contact)

class PyEntityAccessListener(pygame.EntityAccessListener):
    def __init__(self, entity, callback):
        pygame.EntityAccessListener.__init__(self)
        self.entity = entity
        self.callback = callback

    def OnAccessed(self, accessor):
        if self.callback:
            self.callback(self.entity, accessor)
