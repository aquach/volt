import pygame

'''Handy method to get scene pointer.'''
def scene():
    return pygame.Game.Instance().currentScene()

def dt():
    return pygame.AppTime.Instance().dt()

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
