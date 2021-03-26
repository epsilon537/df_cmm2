OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE BOTH

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

DIM objs%(4)
DIM ii%

GM_startUp

IF NOT WM_is_started% THEN
  Error "WM startup error."
ENDIF

DO WHILE ii% < 5
  objs%(ii%) = TestObject_create%()
  'WM_insertObject(objs%(ii%)) No longer needed. Already inserted.

  INC ii%
LOOP

DIM lid% = ObjList_create%()

WM_getAllObjects(lid%, 1)

DIM iter%

ii% = 0
ObjListIter_init(iter%, lid%)
DO WHILE NOT ObjListIter_isDone%(iter%)
  IF (ii% >= 5) THEN
    Error "getAllObjects error."
  ENDIF
  
  PRINT "Current obj: "+STR$(ObjListIter_currentObject%(iter%))
  PRINT "objs: "+STR$(ii%)+" "+STR$(objs%(ii%))
  INC ii%
  ObjListIter_next(iter%)
LOOP

IF ii%<>5 THEN
  Error "getAllObjects error."
ENDIF

ObjList_destroy(lid%)

'No longer needed. Removed from WM upon Object destroy.
'DIM remRes% = WM_removeObject%(objs%(2))
'IF remRes% <> 0 THEN
'  Error "WM_removeObject error."
'ENDIF

Object_destroy(objs%(2), 1)

lid% = ObjList_create%()

WM_getAllObjects(lid%, 1)

ii% = 0
ObjListIter_init(iter%, lid%)
DO WHILE NOT ObjListIter_isDone%(iter%)
  IF (ii% >= 4) THEN
    Error "getAllObjects error."
  ENDIF
  
  PRINT "Current obj: "+STR$(ObjListIter_currentObject%(iter%))
  PRINT "objs: "+STR$(ii%)+" "+STR$(objs%(ii%))
  INC ii%
  ObjListIter_next(iter%)
LOOP

IF ii%<>4 THEN
  Error "getAllObjects error."
ENDIF

ObjList_destroy(lid%)

Object_setType(objs%(1), OBJ_TYPE_OBJECT%)
Object_setType(objs%(3), OBJ_TYPE_OBJECT%)

lid% = WM_objectsOfType%(OBJ_TYPE_OBJECT%)
ObjListIter_init(iter%, lid%)

IF ObjListIter_currentObject%(iter%) <> objs%(1) THEN
  PRINT "currObj: "+STR$(ObjListIter_currentObject%(iter%))
  PRINT "obj: "+STR$(objs%(1))
  Error "objectsOfType error."
ENDIF

ObjListIter_next(iter%)

IF ObjListIter_currentObject%(iter%) <> objs%(3) THEN
  Error "objectsOfType error."
ENDIF

ObjListIter_next(iter%)

IF NOT ObjListIter_isDone%(iter%) THEN
  Error "objectsOfType error."
ENDIF

ObjList_destroy(lid%)

Object_setType(objs%(1), OBJ_TYPE_TEST%)
Object_setType(objs%(3), OBJ_TYPE_TEST%)

PRINT "Marking obj. 3 and 1 for delete."
WM_markForDelete(objs%(3))
WM_markForDelete(objs%(1))

PRINT "Updating WM."
WM_update

PRINT "Updating WM again (should have no effect)"
WM_update

PRINT "Shutting down GM."
GM_shutDown

IF WM_is_started% THEN
  Error "WM shutDown error."
ENDIF

IF num_objLists_allocated%<> 0 THEN
  PRINT "Num ObjLists allocated: "+STR$(num_objLists_allocated%)
  Error "ObjectList leak."
ENDIF

IF num_objs_allocated%<> 0 THEN
  PRINT "Num Objs allocated: "+STR$(num_objs_allocated%)
  Error "Object leak."
ENDIF

PRINT "Test Completed."
END

FUNCTION TestObject_create%()
  LOCAL obj_id% = Object_create%()
  Object_setType(obj_id%, OBJ_TYPE_TEST%)
  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Object_destroy(obj_id%, 0) 'Destroy base class object
END SUB

