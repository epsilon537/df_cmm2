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
#INCLUDE "../INC/Utility.inc"
#INCLUDE "../INC/Animation.inc"
#INCLUDE "../INC/ViewEventTags.inc"
#INCLUDE "../INC/EventView.inc"
#INCLUDE "../INC/ViewObject.inc"
#INCLUDE "../INC/TextEntry.inc"
#INCLUDE "../INC/Object.inc"
#INCLUDE "../INC/ObjectList.inc"
#INCLUDE "../INC/ObjectListIterator.inc"
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

DIM obj1% = TestObject_create%(ASC("y"), ASC("*"), 0)
DIM obj2% = TestObject_create%(ASC("b"), ASC("X"), 1)

GM_run

TestObject_destroy%(obj1%)
TestObject_destroy%(obj2%)

LM_writeLog("Test Complete.")

GM_shutdown

END

FUNCTION TestObject_create%(colr%, c%, focus%)
  LOCAL obj_id% = Object_create%()
  LOCAL extra% = (focus%<<31) OR (c%<<24) OR (colr% AND &HFFFFFF)
  LM_writeLog("focus "+HEX$(focus%))
  LM_writeLog("c "+HEX$(c%))
  LM_writeLog("colr "+HEX$(colr%))
  LM_writeLog("extra "+HEX$(extra%))
  Object_setExtra(obj_id%, extra%)
  Object_setType(obj_id%, OBJ_TYPE_TEST%)
  
  Object_registerInterest(obj_id%, EVT_STEP%)
  Object_registerInterest(obj_id%, EVT_KBD%)

  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

FUNCTION TestObject_eventHandler%(obj_id%, ev%)
  TestObject_eventHandler% = 0
  
  LOCAL eType% = Event_getType%(ev%)
  LOCAL res%
  
  IF eType% = EVT_STEP% THEN
    boxIntersectBox(Object_getBox%(obj1%), Object_getBox%(obj2%), res%)
    IF res% THEN
      LM_writeLog("X")
    ELSE
      LM_writeLog("O")
    ENDIF
  ENDIF
  
  IF eType% = EVT_KBD% THEN
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

      IF NOT focus% THEN
        EXIT FUNCTION
      ENDIF
            
      SELECT CASE key%
      CASE LEFT_KEY%
        INC box_corner%(box_id%), Vector_create%(-MM.INFO(FONTWIDTH),0)
      CASE RIGHT_KEY%
        INC box_corner%(box_id%), Vector_create%(MM.INFO(FONTWIDTH),0)
      CASE UP_KEY%
        INC box_corner%(box_id%), Vector_create%(0,-MM.INFO(FONTHEIGHT))
      CASE DOWN_KEY%
        INC box_corner%(box_id%), Vector_create%(0,MM.INFO(FONTHEIGHT))
      CASE PLUS_KEY%
        INC box_horizontal%(box_id%), MM.INFO(FONTWIDTH)
      CASE MINUS_KEY%
        INC box_horizontal%(box_id%), -MM.INFO(FONTWIDTH)
      CASE SLASH_KEY%
        INC box_vertical%(box_id%), -MM.INFO(FONTHEIGHT)
      CASE COMMA_KEY%
        INC box_vertical%(box_id%), MM.INFO(FONTHEIGHT)
      END SELECT
    ENDIF
  ENDIF
END FUNCTION

SUB TestObject_draw(obj_id%)
  LOCAL x%=0, y%=0
  LOCAL box_id% = Object_getBox%(obj_id%)
  LOCAL corner% = box_corner%(box_id%)
  LOCAL extra% = Object_getExtra%(obj_id%)
  LOCAL c$ = CHR$((extra%>>24) AND 127)
  LOCAL colr% = extra% AND &HFFFFFF
  
  IF colr% = ASC("y") THEN
    colr% = RGB(yellow)
  ELSE
    colr% = RGB(blue)
  ENDIF

  DO WHILE x% < box_horizontal%(box_id%)
    y%=0
    DO WHILE y% < box_vertical%(box_id%)
      DM_drawCh(Vector_create%(x%+INT(Vector_getX!(corner%)), y%+INT(Vector_getY!(corner%))), c$, colr%)

      'TEXT x%+INT(Vector_getX!(corner%)), y%+INT(Vector_getY!(corner%)), c$,LT,,,colr%
      INC y%, MM.INFO(FONTHEIGHT)
    LOOP
    INC x%, MM.INFO(FONTWIDTH)
  LOOP
END SUB

