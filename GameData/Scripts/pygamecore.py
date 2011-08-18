import threading
import time
import pygame
import entity_factory

"""Provides helper methods available to all modules that import pygame."""

def gameMode():
    """Returns whether or not the program running is the game or the editor.
    Python Entities should generally not run game logic while in editor mode."""
    return pygame.Game.Instance() is not None

def dt():
    return pygame.AppTime.Instance().dt()

def level():
    return pygame.LevelManager.Instance()

def disown(swigObj):
    """Sets the flag on a given Python proxy SWIG object such that Python no
    longer owns it."""
    swigObj.thisown = 0
    return swigObj

def longSleep(duration):
    """Sleep, guarding against the level unloading while sleeping."""
    while not level().IsUnloading() and duration > 0:
        sleepTime = min(duration, 1)
        duration -= sleepTime
        time.sleep(sleepTime)

def sleepFrame(tries=25):
    """Sleep until the frame counter has changed, indicating a frame has
    passed. There can be an issue where a script is sleeping, waiting for
    the game to progress, but the game is waiting for the script to finish,
    so limit the time spent waiting."""
    currentFrame = pygame.Game.Instance().frameNumber()
    while currentFrame == pygame.Game.Instance().frameNumber() and tries > 0:
        time.sleep(0.01)
        tries -= 1

def messageBox(message, wait=False, modal=True, canSkip=True,
               pauseDuration=1.0):
    """Wrapper message box generator."""
    b = pygame.MessageBoxDef()
    b.text = message
    b.pauseDuration = pauseDuration
    b.modal = modal
    b.canSkip = canSkip
    
    box = disown(pygame.MessageBox(b))
    pygame.scene().ShowDialogBox(box)
    if wait:
        box.WaitForFinish()

def choiceBox(message, choices):
    """Wrapper choice dialog generator. Returns the index of the chosen
    value."""
    c = pygame.ChoiceBoxDef()
    c.text = message
    c.choices = pygame.StringVector(choices)
    choice = disown(pygame.ChoiceBox(c))
    pygame.scene().ShowDialogBox(choice)
    return choice.WaitForChoice()
    
def background(f):
    """Threading decorator.
    
    Use @background above the function you want to thread (run in the
    background)."""
    def bg_f(*a, **kw):
        thread = threading.Thread(name=f.__name__, target=f, args=a, kwargs=kw)
        thread.start()
        return thread
    return bg_f

def _addInfo(entity):
    # Automatically add tag with the class name.
    entity.AddTag(entity.__class__.__name__)

    # Add type based on entity_factory.
    entityTypeId = entity_factory.ENTITY_TYPES_BY_NAME.get(entity.__class__.__name__, 0)
    if entityTypeId != 0:
        entity.SetType(str(entityTypeId[0]))
            
class PyEntity(pygame.Entity):
    """Wrapper class for defining your own entities."""
    def __init__(self, disown=True):
        pygame.Entity.__init__(self)

        _addInfo(self)
        
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

    def PreSolve(self, other):
        return True
        
    def CanCollideWith(self, other):
        return True

    def Clone(self):
        return None

    def OnScaleChanged(self):
        pass

class PyCreature(pygame.Creature):
    def __init__(self, disown=True):
        pygame.Creature.__init__(self)
        _addInfo(self)
        if disown:
            self.__disown__()

    def Update(self):
        pass

    def Render(self):
        pass

    def CanCollideWith(self, other):
        return True

    def PreSolve(self, other):
        return True

    def Clone(self):
        return None

    def OnScaleChanged(self):
        pass
        
class PyEntityContactListener(pygame.EntityContactListener):
    """Python version of EntityContactListener that can call Python
    callbacks."""
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
    """Python version of EntityAccessListener that can call Python
    callbacks."""
    def __init__(self, entity, callback):
        pygame.EntityAccessListener.__init__(self)
        self.entity = entity
        self.callback = callback

    def OnAccessed(self, accessor):
        if self.callback:
            self.callback(self.entity, accessor)

def addSavePoint(entity):
    """Turns the given entity into a save point."""
    
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
