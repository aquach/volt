# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.40
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.
# This file is compatible with both classic and new-style classes.

from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_pygame', [dirname(__file__)])
        except ImportError:
            import _pygame
            return _pygame
        if fp is not None:
            try:
                _mod = imp.load_module('_pygame', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _pygame = swig_import_helper()
    del swig_import_helper
else:
    import _pygame
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class VoltEntity(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, VoltEntity, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, VoltEntity, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pygame.delete_VoltEntity
    __del__ = lambda self : None;
    def layer(self): return _pygame.VoltEntity_layer(self)
    def IsOnLayer(self, *args): return _pygame.VoltEntity_IsOnLayer(self, *args)
    def SetLayer(self, *args): return _pygame.VoltEntity_SetLayer(self, *args)
    def Clone(self): return _pygame.VoltEntity_Clone(self)
    def CopyFrom(self, *args): return _pygame.VoltEntity_CopyFrom(self, *args)
    def scene(self): return _pygame.VoltEntity_scene(self)
    def HasTag(self, *args): return _pygame.VoltEntity_HasTag(self, *args)
    def AddTag(self, *args): return _pygame.VoltEntity_AddTag(self, *args)
    def RemoveTag(self, *args): return _pygame.VoltEntity_RemoveTag(self, *args)
    def GetTags(self, *args): return _pygame.VoltEntity_GetTags(self, *args)
    def numTags(self): return _pygame.VoltEntity_numTags(self)
    def Update(self): return _pygame.VoltEntity_Update(self)
    def Render(self): return _pygame.VoltEntity_Render(self)
    def OnAdded(self): return _pygame.VoltEntity_OnAdded(self)
    def OnRemoved(self): return _pygame.VoltEntity_OnRemoved(self)
    def UpdatePhysics(self): return _pygame.VoltEntity_UpdatePhysics(self)
    def CreateBody(self, *args): return _pygame.VoltEntity_CreateBody(self, *args)
    def DestroyBody(self): return _pygame.VoltEntity_DestroyBody(self)
    def body(self): return _pygame.VoltEntity_body(self)
    def BeginContact(self, *args): return _pygame.VoltEntity_BeginContact(self, *args)
    def EndContact(self, *args): return _pygame.VoltEntity_EndContact(self, *args)
    def PreSolve(self, *args): return _pygame.VoltEntity_PreSolve(self, *args)
    def CanCollideWith(self, *args): return _pygame.VoltEntity_CanCollideWith(self, *args)
    def RemoveSelf(self): return _pygame.VoltEntity_RemoveSelf(self)
    def transform(self): return _pygame.VoltEntity_transform(self)
    def position(self): return _pygame.VoltEntity_position(self)
    def SetPosition(self, *args): return _pygame.VoltEntity_SetPosition(self, *args)
    def rotation(self): return _pygame.VoltEntity_rotation(self)
    def SetRotation(self, *args): return _pygame.VoltEntity_SetRotation(self, *args)
    def scale(self): return _pygame.VoltEntity_scale(self)
    def SetScale(self, *args): return _pygame.VoltEntity_SetScale(self, *args)
    def OnScaleChanged(self): return _pygame.VoltEntity_OnScaleChanged(self)
    def Load(self, *args): return _pygame.VoltEntity_Load(self, *args)
    def Save(self, *args): return _pygame.VoltEntity_Save(self, *args)
VoltEntity_swigregister = _pygame.VoltEntity_swigregister
VoltEntity_swigregister(VoltEntity)


def __lshift__(*args):
  return _pygame.__lshift__(*args)
__lshift__ = _pygame.__lshift__
class Entity(VoltEntity):
    __swig_setmethods__ = {}
    for _s in [VoltEntity]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Entity, name, value)
    __swig_getmethods__ = {}
    for _s in [VoltEntity]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, Entity, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pygame.delete_Entity
    __del__ = lambda self : None;
    def OnAccessed(self, *args): return _pygame.Entity_OnAccessed(self, *args)
    def GetProperties(self, *args): return _pygame.Entity_GetProperties(self, *args)
    def SetOccludesLight(self, *args): return _pygame.Entity_SetOccludesLight(self, *args)
    def occludesLight(self): return _pygame.Entity_occludesLight(self)
    def Load(self, *args): return _pygame.Entity_Load(self, *args)
    def Save(self, *args): return _pygame.Entity_Save(self, *args)
    def CopyFrom(self, *args): return _pygame.Entity_CopyFrom(self, *args)
Entity_swigregister = _pygame.Entity_swigregister
Entity_swigregister(Entity)

class CreatureHitListener(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CreatureHitListener, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CreatureHitListener, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def OnHit(self, *args): return _pygame.CreatureHitListener_OnHit(self, *args)
    __swig_destroy__ = _pygame.delete_CreatureHitListener
    __del__ = lambda self : None;
CreatureHitListener_swigregister = _pygame.CreatureHitListener_swigregister
CreatureHitListener_swigregister(CreatureHitListener)

class Creature(Entity):
    __swig_setmethods__ = {}
    for _s in [Entity]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Creature, name, value)
    __swig_getmethods__ = {}
    for _s in [Entity]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, Creature, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pygame.delete_Creature
    __del__ = lambda self : None;
    def Update(self): return _pygame.Creature_Update(self)
    def Render(self): return _pygame.Creature_Render(self)
    def EquipWeapon(self, *args): return _pygame.Creature_EquipWeapon(self, *args)
    def AddHitListener(self, *args): return _pygame.Creature_AddHitListener(self, *args)
    def RemoveHitListener(self, *args): return _pygame.Creature_RemoveHitListener(self, *args)
    def weaponTransform(self): return _pygame.Creature_weaponTransform(self)
Creature_swigregister = _pygame.Creature_swigregister
Creature_swigregister(Creature)

class LightStroke(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, LightStroke, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, LightStroke, name)
    __repr__ = _swig_repr
    __swig_setmethods__["vertices"] = _pygame.LightStroke_vertices_set
    __swig_getmethods__["vertices"] = _pygame.LightStroke_vertices_get
    if _newclass:vertices = _swig_property(_pygame.LightStroke_vertices_get, _pygame.LightStroke_vertices_set)
    __swig_setmethods__["color"] = _pygame.LightStroke_color_set
    __swig_getmethods__["color"] = _pygame.LightStroke_color_get
    if _newclass:color = _swig_property(_pygame.LightStroke_color_get, _pygame.LightStroke_color_set)
    def Load(self, *args): return _pygame.LightStroke_Load(self, *args)
    def Save(self, *args): return _pygame.LightStroke_Save(self, *args)
    def __init__(self): 
        this = _pygame.new_LightStroke()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _pygame.delete_LightStroke
    __del__ = lambda self : None;
LightStroke_swigregister = _pygame.LightStroke_swigregister
LightStroke_swigregister(LightStroke)

class Light(Entity):
    __swig_setmethods__ = {}
    for _s in [Entity]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Light, name, value)
    __swig_getmethods__ = {}
    for _s in [Entity]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, Light, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _pygame.new_Light()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _pygame.delete_Light
    __del__ = lambda self : None;
    def Clone(self): return _pygame.Light_Clone(self)
    def CopyFrom(self, *args): return _pygame.Light_CopyFrom(self, *args)
    def OnAdded(self): return _pygame.Light_OnAdded(self)
    def OnRemoved(self): return _pygame.Light_OnRemoved(self)
    def Update(self): return _pygame.Light_Update(self)
    def Render(self): return _pygame.Light_Render(self)
    def Load(self, *args): return _pygame.Light_Load(self, *args)
    def Save(self, *args): return _pygame.Light_Save(self, *args)
    def OnScaleChanged(self): return _pygame.Light_OnScaleChanged(self)
    def GetProperties(self, *args): return _pygame.Light_GetProperties(self, *args)
    def color(self): return _pygame.Light_color(self)
    def maxDistance(self): return _pygame.Light_maxDistance(self)
    def coneAngle(self): return _pygame.Light_coneAngle(self)
    def InvalidateStaticMap(self): return _pygame.Light_InvalidateStaticMap(self)
    def GenerateStrokes(self): return _pygame.Light_GenerateStrokes(self)
    def renderBounds(self): return _pygame.Light_renderBounds(self)
    __swig_setmethods__["STATIC_MAP_FOLDER"] = _pygame.Light_STATIC_MAP_FOLDER_set
    __swig_getmethods__["STATIC_MAP_FOLDER"] = _pygame.Light_STATIC_MAP_FOLDER_get
    if _newclass:STATIC_MAP_FOLDER = _swig_property(_pygame.Light_STATIC_MAP_FOLDER_get, _pygame.Light_STATIC_MAP_FOLDER_set)
Light_swigregister = _pygame.Light_swigregister
Light_swigregister(Light)
cvar = _pygame.cvar

class Player(Creature):
    __swig_setmethods__ = {}
    for _s in [Creature]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Player, name, value)
    __swig_getmethods__ = {}
    for _s in [Creature]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, Player, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _pygame.new_Player()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _pygame.delete_Player
    __del__ = lambda self : None;
    def Update(self): return _pygame.Player_Update(self)
    def Render(self): return _pygame.Player_Render(self)
    def OnKeyEvent(self, *args): return _pygame.Player_OnKeyEvent(self, *args)
    def OnAdded(self): return _pygame.Player_OnAdded(self)
    def OnRemoved(self): return _pygame.Player_OnRemoved(self)
    def BeginContact(self, *args): return _pygame.Player_BeginContact(self, *args)
    def EndContact(self, *args): return _pygame.Player_EndContact(self, *args)
    def IsOnGround(self): return _pygame.Player_IsOnGround(self)
    def SetInputLock(self, *args): return _pygame.Player_SetInputLock(self, *args)
Player_swigregister = _pygame.Player_swigregister
Player_swigregister(Player)

class BrushStroke(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, BrushStroke, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, BrushStroke, name)
    __repr__ = _swig_repr
    __swig_setmethods__["transform"] = _pygame.BrushStroke_transform_set
    __swig_getmethods__["transform"] = _pygame.BrushStroke_transform_get
    if _newclass:transform = _swig_property(_pygame.BrushStroke_transform_get, _pygame.BrushStroke_transform_set)
    __swig_setmethods__["color"] = _pygame.BrushStroke_color_set
    __swig_getmethods__["color"] = _pygame.BrushStroke_color_get
    if _newclass:color = _swig_property(_pygame.BrushStroke_color_get, _pygame.BrushStroke_color_set)
    def __init__(self): 
        this = _pygame.new_BrushStroke()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _pygame.delete_BrushStroke
    __del__ = lambda self : None;
BrushStroke_swigregister = _pygame.BrushStroke_swigregister
BrushStroke_swigregister(BrushStroke)

class Triangle(Entity):
    __swig_setmethods__ = {}
    for _s in [Entity]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, Triangle, name, value)
    __swig_getmethods__ = {}
    for _s in [Entity]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, Triangle, name)
    __repr__ = _swig_repr
    NUM_VERTS = _pygame.Triangle_NUM_VERTS
    def __init__(self): 
        this = _pygame.new_Triangle()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _pygame.delete_Triangle
    __del__ = lambda self : None;
    def Clone(self): return _pygame.Triangle_Clone(self)
    def CopyFrom(self, *args): return _pygame.Triangle_CopyFrom(self, *args)
    def Update(self): return _pygame.Triangle_Update(self)
    def Render(self): return _pygame.Triangle_Render(self)
    def Load(self, *args): return _pygame.Triangle_Load(self, *args)
    def Save(self, *args): return _pygame.Triangle_Save(self, *args)
    def selectedVertex(self, *args): return _pygame.Triangle_selectedVertex(self, *args)
    def OnScaleChanged(self): return _pygame.Triangle_OnScaleChanged(self)
    def GetProperties(self, *args): return _pygame.Triangle_GetProperties(self, *args)
Triangle_swigregister = _pygame.Triangle_swigregister
Triangle_swigregister(Triangle)



