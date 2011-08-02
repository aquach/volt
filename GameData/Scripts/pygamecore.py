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
