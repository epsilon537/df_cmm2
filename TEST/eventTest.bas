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

DIM event%

Event_init(event%)

PRINT "Default event type "+EVT_NAMES$(Event_getType%(event%))

Event_setType(event%, EVT_TEST%)

PRINT "Event type after set "+EVT_NAMES$(Event_getType%(event%))

DIM eventStep%

EventStep_init(eventStep%, 10)

PRINT "Event step type "+EVT_NAMES$(Event_getType%(eventStep%))

IF EventStep_getStepCount%(eventStep%) <> 10 THEN
  Error "EventStep count error."
ENDIF

DIM eventStep2%

EventStep_init(eventStep2%, 20)

PRINT "Event step 2 type "+EVT_NAMES$(Event_getType%(eventStep2%))

IF EventStep_getStepCount%(eventStep2%) <> 20 THEN
  Error "EventStep count error."
ENDIF

DIM testObject% = TestObject_create%()

PRINT "Test Object type "+OBJ_NAMES$(Object_getType%(testObject%))

DIM ehRes% = Object_eventHandler%(testObject%, event%)
ehRes% = Object_eventHandler%(testObject%, eventStep%)
ehRes% = Object_eventHandler%(testObject%, eventStep2%)

Object_destroy(testObject%, 1)

IF num_objs_allocated%<> 0 THEN
  ERROR "Object leak. Num Objs allocated: "+STR$(num_objs_allocated%)
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
  Object_destroy(obj_id%, 0)
END SUB

FUNCTION TestObject_eventHandler%(obj_id%, ev%)
  PRINT "TestObject eventHandler received event "+EVT_NAMES$(Event_getType%(ev%))
  TestObject_eventHandler% = 1
END FUNCTION

