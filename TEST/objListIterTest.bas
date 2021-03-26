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

DIM objList%, objList2%

GM_startUp

objList% = ObjList_create%()
objList2% = ObjList_create%()

DIM obj%(4)
obj%(0) = Object_create%()
obj%(1) = Object_create%()
obj%(2) = Object_create%()
obj%(3) = Object_create%()
obj%(4) = Object_create%()

ObjList_insert(objList%, obj%(0))
ObjList_insert(objList%, obj%(1))
ObjList_insert(objList%, obj%(2))
ObjList_insert(objList%, obj%(3))
ObjList_insert(objList%, obj%(4))

DIM iter%, iter2%

ObjListIter_init(iter%, objList%)
ObjListIter_init(iter2%, objList2%)

IF ObjListIter_currentObject%(iter%) <> obj%(0) THEN
  Error "currentObject error"
ENDIF

ObjListIter_next(iter%)

IF ObjListIter_currentObject%(iter%) <> obj%(1) THEN
  Error "currentObject error"
ENDIF

ObjListIter_first(iter%)
IF ObjListIter_currentObject%(iter%) <> obj%(0) THEN
  Error "currentObject error"
ENDIF

IF ObjListIter_isDone%(iter%) THEN
  Error "isDone error"
ENDIF

ObjListIter_next(iter%)

IF ObjListIter_isDone%(iter%) THEN
  Error "isDone error"
ENDIF

ObjListIter_next(iter%)

IF ObjListIter_isDone%(iter%) THEN
  Error "isDone error"
ENDIF

ObjListIter_next(iter%)

IF ObjListIter_isDone%(iter%) THEN
  Error "isDone error"
ENDIF

ObjListIter_next(iter%)

IF ObjListIter_isDone%(iter%) THEN
  Error "isDone error"
ENDIF

PRINT "First Pass:"
ObjListIter_first(iter%)
DO WHILE NOT ObjListIter_isDone%(iter%)
  PRINT ObjListIter_currentObject%(iter%)
  ObjListIter_next(iter%)
LOOP

PRINT "2nd Pass:"
ObjListIter_first(iter%)
DO WHILE NOT ObjListIter_isDone%(iter%)
  PRINT ObjListIter_currentObject%(iter%)
  ObjListIter_next(iter%)
LOOP

ObjList_copy(objList%, objList2%)
PRINT "Copied to 2nd list:"
ObjListIter_first(iter2%)
DO WHILE NOT ObjListIter_isDone%(iter2%)
  PRINT ObjListIter_currentObject%(iter2%)
  ObjListIter_next(iter2%)
LOOP

ObjList_append(objList2%, objList%)
PRINT "After append:"
ObjListIter_first(iter2%)
DO WHILE NOT ObjListIter_isDone%(iter2%)
  PRINT ObjListIter_currentObject%(iter2%)
  ObjListIter_next(iter2%)
LOOP

PRINT "Halfway inserting object:"
DIM ii%=0
DIM objX% = Object_create%()

ObjListIter_first(iter2%)
DO WHILE NOT ObjListIter_isDone%(iter2%)
  IF ii%=2 THEN
    ObjList_insert(objList2%, objX%)
  ENDIF
  
  PRINT ObjListIter_currentObject%(iter2%)
  ObjListIter_next(iter2%)
  INC ii%
LOOP

PRINT "Halfway removing object:"
ii%=0
DIM res%

ObjListIter_first(iter2%)
DO WHILE NOT ObjListIter_isDone%(iter2%)
  IF ii%=2 THEN
    res%=ObjList_remove%(objList2%, obj%(4))
  ENDIF
  
  PRINT ObjListIter_currentObject%(iter2%)
  ObjListIter_next(iter2%)
  INC ii%
LOOP

PRINT "Halfway clearing list:"
ii%=0

ObjListIter_first(iter2%)
DO WHILE NOT ObjListIter_isDone%(iter2%)
  IF ii%=2 THEN
    ObjList_clear(objList2%)
  ENDIF
  
  PRINT ObjListIter_currentObject%(iter2%)
  ObjListIter_next(iter2%)
  INC ii%
LOOP

PRINT "Empty list loop:"
ObjListIter_first(iter2%)
DO WHILE NOT ObjListIter_isDone%(iter2%)
  PRINT ObjListIter_currentObject%(iter2%)
  ObjListIter_next(iter2%)
LOOP

ObjList_destroy(objList%)
ObjList_destroy(objList2%)

Object_destroy(obj%(0), 1)
Object_destroy(obj%(1), 1)
Object_destroy(obj%(2), 1)
Object_destroy(obj%(3), 1)
Object_destroy(obj%(4), 1)
Object_destroy(objX%, 1)

IF num_objs_allocated%<> 0 THEN
  PRINT "Num Objs allocated: "+STR$(num_objs_allocated%)
  ERROR "Object leak."
ENDIF

GM_shutdown

IF num_objLists_allocated%<> 0 THEN
  PRINT "Num ObjLists allocated: "+STR$(num_objLists_allocated%)
  ERROR "ObjectList leak."
ENDIF

PRINT "Test Completed."
END

