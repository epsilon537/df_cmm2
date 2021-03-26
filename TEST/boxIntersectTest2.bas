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

RM_loadSprite("../sprites/orig.txt", "Saucer")
RM_loadSprite("../sprites/ship-spr.txt", "Ship")

DIM obj1% = TestObject_create%(ASC("y"), ASC("*"), 0)
DIM obj2% = TestObject_create%(ASC("b"), ASC("X"), 1)

Object_setSprite(obj1%, "Saucer")
Object_setSprite(obj2%, "Ship")

Object_setSolidness(obj1%, OBJ_SOLID_SOFT%)
Object_setSolidness(obj2%, OBJ_SOLID_SOFT%)

DIM v%
Vector_setXY(v%, DM_getHorizontal%()*0.25, DM_getVertical%()*0.25)
obj_position%(obj1%) = v%
Vector_setXY(v%, DM_getHorizontal%()*0.75, DM_getVertical%()*0.75)
obj_position%(obj2%) = v%

GM_run

TestObject_destroy%(obj1%)
TestObject_destroy%(obj2%)

LM_writeLog("Test Complete.")

GM_shutdown

END

FUNCTION TestObject_create%(colr%, c%, focus%)
  LOCAL obj_id% = Object_create%()
  LOCAL extra% = (focus%<<31) OR (c%<<24) OR (colr% AND &HFFFFFF)
  Object_setExtra(obj_id%, extra%)
  Object_setType(obj_id%, OBJ_TYPE_TEST%)
  
  Object_registerInterest(obj_id%, EVT_STEP%)
  Object_registerInterest(obj_id%, EVT_KBD%)
  Object_registerInterest(obj_id%, EVT_COL%)
  
  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

FUNCTION TestObject_eventHandler%(obj_id%, ev%)
  TestObject_eventHandler% = 0
  
  LOCAL eType% = Event_getType%(ev%)
  
  IF eType% = EVT_STEP% THEN
    STATIC wBox% = Box_create%(0,0,0)
    
    LOCAL center% = Vector_create%(DM_getHorizontal%()*0.5, DM_getVertical%()*0.5)
    
    TEXT DM_getHorizontal%()*0.5, DM_getVertical%()*0.5, "+", LT,,,RGB(white)
    
    getWorldBox(obj_id%, wBox%)
    
    IF boxContainsPosition%(wBox%, center%) THEN
      LM_writeLog("!")
    ENDIF  
    
    STATIC wBox1% = Box_create%(0,0,0)
    STATIC wBox2% = Box_create%(0,0,0)

    getWorldBox(obj1%, wBox1%)
    getWorldBox(obj2%, wBox2%)

    IF boxContainsBox%(wBox1%, wBox2%) THEN
      LM_writeLog("?")
    ENDIF

    IF boxContainsBox%(wBox2%, wBox1%) THEN
      LM_writeLog(".")
    ENDIF    
  ENDIF

  IF eType% = EVT_COL% THEN
    LM_writeLog "X"
  ENDIF    
  
  IF eType% = EVT_KBD% THEN
    IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
      LOCAL key% = EVT_KBD_getKey%(ev%) 
      LOCAL extra% = Object_getExtra%(obj_id%)
      LOCAL focus% = extra%>>31
      LOCAL box_id% = Object_getBox%(obj_id%)

      LM_writeLog("Key pressed: "+STR$(key%))
            
      IF key% = F_KEY% THEN
        focus% = NOT focus%
        extra% = (focus%<<31) OR (extra% AND &H7FFFFFFF)
        Object_setExtra(obj_id%, extra%)
      ENDIF

      IF NOT focus% THEN
        EXIT FUNCTION
      ENDIF
            
      SELECT CASE key%
      CASE T_KEY%
        IF obj_id% = obj1% THEN
          Object_setActive(obj2%, NOT Object_isActive%(obj2%))
        ELSE
          Object_setActive(obj1%, NOT Object_isActive%(obj1%))        
        ENDIF
      CASE X_KEY%
        LM_writeLog("solid spectral")
        Object_setSolidness(obj_id%, OBJ_SOLID_SPECTRAL%)        
      CASE H_KEY%
        LM_writeLog("solid hard")
        Object_setSolidness(obj_id%, OBJ_SOLID_HARD%)
      CASE W_KEY%
        Object_setVelocity(obj_id%, Object_getVelocity%(obj_id%)+Vector_create%(0,-1))  
      CASE A_KEY%
        Object_setVelocity(obj_id%, Object_getVelocity%(obj_id%)+Vector_create%(-1,0))  
      CASE S_KEY%
        Object_setVelocity(obj_id%, Object_getVelocity%(obj_id%)+Vector_create%(0,1))  
      CASE D_KEY%
        Object_setVelocity(obj_id%, Object_getVelocity%(obj_id%)+Vector_create%(1,0))  
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
        INC box_vertical%(box_id%),-MM.INFO(FONTHEIGHT)
      CASE COMMA_KEY%
        INC box_vertical%(box_id%), MM.INFO(FONTHEIGHT)
      END SELECT
    ENDIF
  ENDIF
END FUNCTION

SUB TestObject_draw(obj_id%)
  STATIC worldBox% = Box_create%(0,0,0)

  Object_draw(obj_id%, 0)
  
  getWorldBox(obj_id%, worldBox%)

  LOCAL wBoxCorner% = box_corner%(worldBox%)
  LOCAL hor% = box_horizontal%(worldBox%)
  LOCAL ver% = box_vertical%(worldBox%)
  LOCAL otherCorner% = wBoxCorner% + Vector_create%(hor%,ver%)

  LOCAL x%=0, y%=0
  LOCAL extra% = Object_getExtra%(obj_id%)
  LOCAL c$ = CHR$((extra%>>24) AND 127)
  LOCAL colr% = extra% AND &HFFFFFF
  
  IF colr% = ASC("y") THEN
    colr% = RGB(yellow)
  ELSE
    colr% = RGB(blue)
  ENDIF

  DO WHILE x% < hor%
    y%=0
    DO WHILE y% < ver%
      DM_drawCh(Vector_create%(x%+INT(Vector_getX!(wBoxCorner%)), y%+INT(Vector_getY!(wBoxCorner%))), c$, colr%)

      'TEXT x%+INT(Vector_getX!(wBoxCorner%)), y%+INT(Vector_getY!(wBoxCorner%)), c$,LT,,,colr%
      INC y%, MM.INFO(FONTHEIGHT)
    LOOP
    INC x%, MM.INFO(FONTWIDTH)
  LOOP

  DM_drawCh(wBoxCorner%, "b", RGB(green))
  DM_drawCh(otherCorner%, "B", RGB(green))
END SUB

