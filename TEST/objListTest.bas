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

GM_startUp

DIM objLid% = ObjList_create%()

IF ObjList_isAllocated%(objLid%) = 0 THEN
  Error "ObjList not allocated."
ENDIF

IF objList_numElems%(objLid%) <> 0 THEN
  Error "ObjList numElems incorrect."
ENDIF

IF NOT ObjList_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error."
ENDIF

IF ObjList_isFull%(objLid%) THEN
  Error "ObjList isFull error."
ENDIF

DIM obj1% = Object_create%()

ObjList_insert(objLid%, obj1%)

IF ObjList_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error (2)."
ENDIF

IF ObjList_isFull%(objLid%) THEN
  Error "ObjList isFull error.(2)"
ENDIF

IF objList_numElems%(objLid%) <> 1 THEN
  Error "ObjList numElems incorrect (2)."
ENDIF

DIM obj2% = Object_create%()
DIM obj3% = Object_create%()

ObjList_insert(objLid%, obj2%)
ObjList_insert(objLid%, obj3%)

IF ObjList_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error (3)."
ENDIF

IF ObjList_isFull%(objLid%) THEN
  Error "ObjList isFull error.(3)"
ENDIF

IF objList_numElems%(objLid%) <> 3 THEN
  Error "ObjList numElems incorrect (3)."
ENDIF

DIM objLid2% = ObjList_create%()

ObjList_copy(objLid%, objLid2%) 'From objLid to objLid2
IF objList_numElems%(objLid2%) <> 3 THEN
  Error "ObjList numElems incorrect (4)."
ENDIF

IF NOT ObjList_equal%(objLid%, objLid2%) THEN
  Error "ObjList copy or compare error."
ENDIF

DIM res% = ObjList_remove%(objLid%, obj2%)
IF res% = -1 THEN
  Error "ObjList remove error."
ENDIF

IF objList_numElems%(objLid%) <> 2 THEN
  Error "ObjList numElems incorrect (4)."
ENDIF

IF ObjList_equal%(objLid%, objLid2%) THEN
  Error "ObjList compare error (2)."
ENDIF

res% = ObjList_remove%(objLid%, obj2%)
IF res% <> -1 THEN
  Error "ObjList remove error (2)."
ENDIF

ObjList_append(objLid2%, objLid%)
IF objList_numElems%(objLid2%) <> 5 THEN
  Error "ObjList numElems incorrect (5)."
ENDIF

IF ObjList_isEmpty%(objLid2%) THEN
  Error "ObjList isEmpty error (5)."
ENDIF

IF ObjList_isFull%(objLid2%) THEN
  Error "ObjList isFull error.(5)"
ENDIF

ObjList_clear(objLid%)
IF NOT ObjList_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error (6)."
ENDIF

DIM ii%=0
DIM objX%
DIM objs%(MAX_NUM_OBJS_IN_LIST%-1)

DO WHILE NOT ObjList_isFull%(objLid%) 'ii% < MAX_NUM_OBJS_IN_LIST%
  objX% = Object_create%()
  objs%(ii%) = objX%
  ObjList_insert(objLid%, objX%)
  INC ii%
LOOP

DIM numObjsInList% = ii%

IF NOT ObjList_isFull%(objLid%) THEN
  Error "ObjList isFull error.(6)"
ENDIF

IF objList_numElems%(objLid%) <> numObjsInList% THEN
  Error "ObjList numElems incorrect (6)."
ENDIF

ii%=0
DO WHILE ii% < numObjsInList%
  objX% = objs%(ii%)
  res% = ObjList_remove%(objLid%, objX%)
  IF res% = -1 THEN
    Error "ObjList remove error (3), ii="+STR$(ii%)+" obj id="+STR$(objX%)
  ENDIF
  INC ii%
LOOP

IF NOT ObjList_isEmpty%(objLid%) THEN
  Error "ObjList isEmpty error (7)."
ENDIF

ObjList_destroy(objLid%)
ObjList_destroy(objLid2%)

IF ObjList_isAllocated%(objLid%) THEN
  Error "ObjList not allocated.(2)"
ENDIF

IF ObjList_isAllocated%(objLid2%) THEN
  Error "ObjList not allocated.(3)"
ENDIF

Object_destroy(obj1%, 1)
Object_destroy(obj2%, 1)
Object_destroy(obj3%, 1)

ii%=0
DO WHILE ii% < numObjsInList%
  Object_destroy(objs%(ii%), 1)
  INC ii%
LOOP

'IF num_objLists_allocated%<> num_objLists_allocated% THEN
'  PRINT "Num ObjLists allocated: "+STR$(num_objLists_allocated%)
'  Error "ObjectList leak."
'ENDIF

IF num_objs_allocated%<> 0 THEN
  PRINT "Num Objs allocated: "+STR$(num_objs_allocated%)
  Error "Object leak."
ENDIF

GM_shutdown

PRINT "Test Completed."

END

