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

RM_loadSprite("../sprites/orig.txt", "Saucer")
RM_loadSprite("../sprites/ship-spr.txt", "Ship")

DIM ship% = TestObject_create%()
DIM saucer% = TestObject_create%()

DIM shipBox% = Object_getBox%(ship%)

IF box_corner%(shipBox%) <> 0 THEN
  ERROR "Box_getCorner error."
ENDIF
IF box_horizontal%(shipBox%) <> MM.INFO(FONTWIDTH) THEN
  ERROR "Box_getHorizontal error."
ENDIF
IF box_vertical%(shipBox%) <> MM.INFO(FONTHEIGHT) THEN
  ERROR "Box_getVertical error."
ENDIF

DIM v%

Object_setSprite(ship%, "Ship")
Vector_setXY(v%, DM_getHorizontal%()*1/3, DM_getVertical%()/2)
obj_position%(ship%) = v%
Vector_setXY(v%, 1.0, 0.5)
Object_setVelocity(ship%, v%)

IF box_corner%(shipBox%) <> Vector_create%(-2*MM.INFO(FONTWIDTH),-1.5*MM.INFO(FONTHEIGHT)) THEN
  ERROR "Box_getCorner error"
ENDIF
IF box_horizontal%(shipBox%) <> 4*MM.INFO(FONTWIDTH) THEN
  ERROR "Box_getHorizontal error."
ENDIF
IF box_vertical%(shipBox%) <> 3*MM.INFO(FONTHEIGHT) THEN
  ERROR "Box_getVertical error."
ENDIF

Object_setSprite(saucer%, "Saucer")
Vector_setXY(v%, DM_getHorizontal%()*2/3, DM_getVertical%()/2)
obj_position%(saucer%) = v%
Vector_setXY(v%, -0.5, 1.0)
Object_setVelocity(saucer%, v%)

DIM saucerBox% = Object_getBox%(saucer%)
IF box_corner%(saucerBox%) <> Vector_create%(-3*MM.INFO(FONTWIDTH),-1*MM.INFO(FONTHEIGHT)) THEN
  ERROR "Box_getCorner error."
ENDIF
IF box_horizontal%(saucerBox%) <> 6*MM.INFO(FONTWIDTH) THEN
  ERROR "Box_getHorizontal error."
ENDIF
IF box_vertical%(saucerBox%) <> 2*MM.INFO(FONTHEIGHT) THEN
  ERROR "Box_getVertical error."
ENDIF

Object_registerInterest(ship%, EVT_KBD%)
Object_registerInterest(ship%, EVT_STEP%)
Object_registerInterest(ship%, EVT_JOY%)

GM_run

RM_unloadSprite("Saucer")
RM_unloadSprite("Ship")

LM_writeLog("Test Complete.")

GM_shutdown

END

FUNCTION TestObject_create%()
  LOCAL obj_id% = Object_create%()
  Object_setType(obj_id%, OBJ_TYPE_TEST%)
  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

SUB TestObject_draw(obj_id%)
  IF Sprite_getLabel$(Anim_getSprite%(Object_getAnim%(obj_id%))) = "Saucer" THEN
    LM_writeLog("Saucer drawn "+STR$(GM_frame_count%))
  ENDIF
  
  Object_draw(obj_id%, 0)
END SUB

FUNCTION TestObject_eventHandler%(obj_id%, ev%)
  SELECT CASE Event_getType%(ev%)
    CASE EVT_OUT%
      LM_writeLog("Event Out for obj "+STR$(obj_id%))
      Object_setVelocity(obj_id%, INV(Object_getVelocity%(obj_id%)))
      
      'TestObject_eventHandler% = 1
      'EXIT FUNCTION
    CASE EVT_STEP%
      LM_writeLog("Obj_id "+STR$(obj_id%)+" Event Step count ="+STR$(EventStep_getStepCount%(ev%)))    
      'TestObject_eventHandler% = 1
      'EXIT FUNCTION
    CASE EVT_KBD%
      IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
        LOCAL key% = EVT_KBD_getKey%(ev%) 
        LOCAL extra% = Object_getExtra%(obj_id%)
        LOCAL focus% = extra%>>31
        LOCAL box_id% = Object_getBox%(obj_id%)
        
        IF key% = F_KEY% THEN
          focus% = NOT focus%
          extra% = (focus%<<31) OR (extra% AND &H7FFFFFFF)
          Object_setExtra(obj_id%, extra%)
        ENDIF
  
        IF focus% THEN
          Object_registerInterest(obj_id%, EVT_STEP%)
        ELSE
          LOCAL dummy% = Object_unregisterInterest%(obj_id%, EVT_STEP%)
        ENDIF
        
        IF key% = Q_KEY% THEN
          Object_destroy(obj_id%)
        ENDIF
      ENDIF
    CASE EVT_JOY%
      LOCAL act% = EVT_JOY_getAction%(ev%)
      LM_writeLog("Joystick event action: "+STR$(act%))
      LOCAL button% = EVT_JOY_getButton%(ev%)
      LM_writeLog("Joystick event button: "+STR$(button%))      
      LOCAL position% = EVT_JOY_getPosition%(ev%)
      LM_writeLog("Joystick event position: X="+STR$(Vector_getX!(position%))+" Y="+STR$(Vector_getY!(position%))) 
  END SELECT
  
  TestObject_eventHandler% = 0
END FUNCTION

