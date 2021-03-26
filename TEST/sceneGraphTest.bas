OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE SERIAL

#INCLUDE "../INC/Frame.inc"
#INCLUDE "../INC/Sprite.inc"
#INCLUDE "../INC/EventTypes.inc"
#INCLUDE "../INC/Event.inc"
#INCLUDE "../INC/EventStep.inc"
#INCLUDE "../INC/Object_types.inc"
#INCLUDE "../INC/Box.inc"
#INCLUDE "../INC/Animation.inc"
#INCLUDE "../INC/ViewEventTags.inc"
#INCLUDE "../INC/EventView.inc"
#INCLUDE "../INC/ViewObject.inc"
#INCLUDE "../INC/TextEntry.inc"
#INCLUDE "../INC/Object.inc"
#INCLUDE "../INC/ObjectList.inc"
#INCLUDE "../INC/ObjectListIterator.inc"
#INCLUDE "../INC/Utility.inc"
#INCLUDE "../INC/EventCollision.inc"
#INCLUDE "../INC/Manager.inc"
#INCLUDE "../INC/LogManager.inc"
#INCLUDE "../INC/DisplayManager.inc"
#INCLUDE "../INC/EventOut.inc"
#INCLUDE "../INC/SceneGraph.inc"
#INCLUDE "../INC/WorldManager.inc"
#INCLUDE "../INC/EventKeyboard.inc"
#INCLUDE "../INC/EventJoystick.inc"
#INCLUDE "../INC/InputManager.inc"
#INCLUDE "../INC/ResourceManager.inc"
#INCLUDE "../INC/Sound.inc"
#INCLUDE "../INC/GameManager.inc"
#INCLUDE "../INC/Vector.inc"

GM_startUp

DIM obj1% = Object_create%()
DIM obj2% = Object_create%()

DIM lid% = ObjList_create%()
WM_getAllObjects(lid%, 0)

IF NOT ObjList_equal%(lid%, active_objects_lid%) THEN
  ERROR "WM_getAllObjects error."
ENDIF

DIM iter%
ObjListIter_init(iter%, lid%)

ObjListIter_first(iter%)
IF ObjListIter_currentObject%(iter%) <> obj1% THEN
  ERROR "SceneGraph active obj error."
ENDIF

ObjListIter_next(iter%)
IF ObjListIter_currentObject%(iter%) <> obj2% THEN
  ERROR "SceneGraph active obj error."
ENDIF

ObjListIter_next(iter%)
IF NOT ObjListIter_isDone%(iter%) THEN
  ERROR "SceneGraph active obj error."
ENDIF

WM_removeObject(obj1%)

ObjListIter_init(iter%, active_objects_lid%)

ObjListIter_first(iter%)
IF ObjListIter_currentObject%(iter%) <> obj2% THEN
  ERROR "SceneGraph remove obj error."
ENDIF
ObjListIter_next(iter%)
IF NOT ObjListIter_isDone%(iter%) THEN
  ERROR "SceneGraph active obj error."
ENDIF

WM_removeObject(obj2%)

WM_insertObject(obj1%)
WM_insertObject(obj2%)

Object_setSolidness(obj1%, OBJ_SOLID_SOFT%)
Object_setAltitude(obj1%, 1)

Object_setSolidness(obj2%, OBJ_SOLID_SPECTRAL%)
Object_setAltitude(obj2%, 2)

ObjListIter_init(iter%, solid_objects_lid%)
ObjListIter_first(iter%)
IF ObjListIter_currentObject%(iter%) <> obj1% THEN
  ERROR "SceneGraph solid obj error."
ENDIF
ObjListIter_next(iter%)
IF NOT ObjListIter_isDone%(iter%) THEN
  ERROR "SceneGraph solid obj error."
ENDIF

ObjListIter_init(iter%, visible_objects_lid%(1))
ObjListIter_first(iter%)
IF ObjListIter_currentObject%(iter%) <> obj1% THEN
  ERROR "SceneGraph visible obj error."
ENDIF
ObjListIter_next(iter%)
IF NOT ObjListIter_isDone%(iter%) THEN
  ERROR "SceneGraph visible obj error."
ENDIF

ObjListIter_init(iter%, visible_objects_lid%(2))
ObjListIter_first(iter%)
IF ObjListIter_currentObject%(iter%) <> obj2% THEN
  ERROR "SceneGraph visible obj error."
ENDIF
ObjListIter_next(iter%)
IF NOT ObjListIter_isDone%(iter%) THEN
  ERROR "SceneGraph visible obj error."
ENDIF

WM_removeObject(obj2%)
WM_removeObject(obj1%)

IF NOT ObjList_isEmpty%(visible_objects_lid%(1)) THEN
  ERROR "SceneGraph visible obj error"
ENDIF

IF NOT ObjList_isEmpty%(visible_objects_lid%(2)) THEN
  ERROR "SceneGraph visible obj error"
ENDIF

IF NOT ObjList_isEmpty%(solid_objects_lid%) THEN
  ERROR "SceneGraph solid obj error"
ENDIF

IF NOT ObjList_isEmpty%(active_objects_lid%) THEN
  ERROR "SceneGraph active obj error"
ENDIF

GM_shutDown

PRINT "Test Done."
END

