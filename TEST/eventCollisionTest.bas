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

DIM obj1% = Object_create%()
DIM obj2% = Object_create%()
DIM v% = Vector_create%(3,4)

DIM ev% = EvtCol_create%(obj1%, obj2%, v%)

IF Event_getType%(ev%) <> EVT_COL% THEN
  Error "Event Type incorrect".
ENDIF

IF EvtCol_getObj1%(ev%) <> obj1% THEN
  Error "EvtCol obj1 incorrect."
ENDIF

IF EvtCol_getObj2%(ev%) <> obj2% THEN
  Error "EvtCol obj2 incorrect."
ENDIF

IF EvtCol_getPos%(ev%) <> v% THEN
  Error "EvtCol position incorrect."
ENDIF

DIM obj3% = Object_create%()
DIM obj4% = Object_create%()
DIM v2% = Vector_create%(5,6)

EvtCol_setObj1(ev%, obj3%)
EvtCol_setObj2(ev%, obj4%)
EvtCol_setPos(ev%, v2%)

IF Event_getType%(ev%) <> EVT_COL% THEN
  Error "Event Type incorrect".
ENDIF

IF EvtCol_getObj1%(ev%) <> obj3% THEN
  Error "EvtCol obj1 incorrect."
ENDIF

IF EvtCol_getObj2%(ev%) <> obj4% THEN
  Error "EvtCol obj2 incorrect."
ENDIF

IF EvtCol_getPos%(ev%) <> v2% THEN
  Error "EvtCol position incorrect."
ENDIF

IF NOT EvtCol_isAllocated%(ev%) THEN
  Error "EvtCol allocation error"
ENDIF

Event_destroy(ev%)
IF EvtCol_isAllocated%(ev%) THEN
  Error "EvtCol destroy error"
ENDIF

PRINT "Test Completed."
END

