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

GM_startup

PRINT "Test started."

DIM testObject% = TestObject_create%()
DIM testObject2% = TestObject_create%()

DO
  IM_getInput
LOOP

END

FUNCTION TestObject_create%()
  LOCAL obj_id% = Object_create%()
  Object_setType(obj_id%, OBJ_TYPE_TEST%)
  Object_registerInterest(obj_id%, EVT_KBD%)
  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

FUNCTION TestObject_eventHandler%(obj_id%, ev%)

  IF Event_getType%(ev%) <> EVT_KBD% THEN
    Error "Incorrect event type"
  ENDIF

  LOCAL action% = EVT_KBD_getAction%(ev%)
  LOCAL key% = EVT_KBD_getKey%(ev%)
  
  SELECT CASE action%
    CASE EVT_KBD_ACT_KEY_PRESSED%
      PRINT "Obj. "+STR$(obj_id%)+" Key pressed: "+STR$(key%)
    CASE EVT_KBD_ACT_KEY_RELEASED%
      PRINT "Obj. "+STR$(obj_id%)+" Key released: "+STR$(key%)
    CASE EVT_KBD_ACT_KEY_DOWN%
      'PRINT STR$(obj_id%)+" Key down: "+STR$(key%)
    CASE ELSE
      Error "Unknown action"
  END SELECT
  
  TestObject_eventHandler% = 1
END FUNCTION

