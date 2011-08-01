import pygame

class Wrapper(object):
    '''
    Object wrapper class.
    This a wrapper for objects. It is initialiesed with the object to wrap
    and then proxies the unhandled getattribute methods to it.
    Other classes are to inherit from it.
    '''
    def __init__(self, obj):
        '''
        Wrapper constructor.
        @param obj: object to wrap
        '''
        # wrap the object
        self._wrapped_obj = obj

    def __getattr__(self, attr):
        # see if this object has attr
        # NOTE do not use hasattr, it goes into
        # infinite recurrsion
        if attr in self.__dict__:
            # this object has it
            return getattr(self, attr)
        # proxy to the wrapped object
        return getattr(self._wrapped_obj, attr)

class Scene(Wrapper):
    def __init__(self, scene):
        Wrapper.__init__(self, scene)

    def GetAllTagged(self, tag):
        entities = pygame.EntityVector()
        self._wrapped_obj.GetAllTagged(tag, entities)
        return entities

'''Handy method to get scene pointer.'''
def scene():
    return Scene(pygame.Game.Instance().currentScene())

def dt():
    return pygame.AppTime.Instance().dt()

class PyEntity(pygame.Entity):
    ''' Wrapper class for defining your own entities.'''
    def __init__(self):
        pygame.Entity.__init__(self)
        # Disown pointer from Python because we're probably going to give
        # it to the Scene to handle.
        self.__disown__()

    def OnAdded(self):
        pass

    def OnRemoved(self):
        pass
        
    def Update(self):
        pass

    def Render(self):
        pass
