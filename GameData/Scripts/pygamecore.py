import pygame
import threading
import time

def dt():
    return pygame.AppTime.Instance().dt()

def level():
    return pygame.LevelManager.Instance()

"""Sets the flag on a given Python proxy SWIG object such that Python no longer
   owns it."""
def disown(swigObj):
    swigObj.thisown = 0
    return swigObj

"""Sleep, guarding against the level unloading while sleeping."""
def longSleep(duration):
    while not level().IsUnloading() and duration > 0:
        sleepTime = min(duration, 1)
        duration -= sleepTime
        time.sleep(sleepTime)

"""Sleep until the frame counter has changed, indicating a frame has passed.
   There can be an issue where a script is sleeping, waiting for the game to
   progress, but the game is waiting for the script to finish, so limit the time
   spent waiting."""
def sleepFrame(tries=25):
    currentFrame = pygame.Game.Instance().frameNumber()
    while currentFrame == pygame.Game.Instance().frameNumber() and tries > 0:
        time.sleep(0.01)
        tries -= 1

"""Wrapper message box generator."""
def messageBox(message, wait=False, modal=True, canSkip=True,
               pauseDuration=1.0):
    b = pygame.MessageBoxDef()
    b.text = message
    b.pauseDuration = pauseDuration
    b.modal = modal
    b.canSkip = canSkip
    
    box = disown(pygame.MessageBox(b))
    pygame.scene().ShowDialogBox(box)
    if wait:
        box.WaitForFinish()

"""Wrapper choice dialog generator. Returns the index of the chosen value."""
def choiceBox(message, choices):
    c = pygame.ChoiceBoxDef()
    c.text = message
    c.choices = pygame.StringVector(choices)
    choice = disown(pygame.ChoiceBox(c))
    pygame.scene().ShowDialogBox(choice)
    return choice.WaitForChoice()
    
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

"""Turns the given entity into a save point."""
def savePoint(entity):
    @background
    def saveDialog(unused_entity, unused_accessor):
        choice = choiceBox("Do you want to save?", ['Yes', 'No'])
        if choice == 0:
            messageBox("Saving...")
            pygame.SaveData.Instance().Save()
            messageBox("Saved.")
        elif choice == 1:
            pass
        else:
            raise Exception('Undefined choice', choice)
        return
        
    entity.OnAccessed(saveDialog)
