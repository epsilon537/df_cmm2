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
#INCLUDE "../INC/xprof.inc"

'GM_frame_time_ms%=17

GM_startUp
LM_writeLog "GM_isStarted="+STR$(GM_is_Started%)
LM_writeLog "LM_isStarted="+STR$(LM_is_Started%)
LM_writeLog "WM_isStarted="+STR$(WM_is_Started%)

DIM obj_frame_count%=0

DIM testObjs%(2)
testObjs%(0) = TestObject_create%()
testObjs%(1) = TestObject_create%()
testObjs%(2) = TestObject_create%()
Object_setExtra(testObjs%(0), RGB(yellow))
Object_setExtra(testObjs%(1), RGB(yellow))
Object_setExtra(testObjs%(2), RGB(yellow))

'DIM moreObjs%(2)
'moreObjs%(0) = TestObject_create%()
'moreObjs%(1) = TestObject_create%()
'moreObjs%(2) = TestObject_create%()

'Object_setAltitude(moreObjs%(0), 1)
'Object_setAltitude(moreObjs%(1), 1)
'Object_setAltitude(moreObjs%(2), 1)
'Object_setExtra(moreObjs%(0), RGB(blue))
'Object_setExtra(moreObjs%(1), RGB(blue))
'Object_setExtra(moreObjs%(2), RGB(blue))

LM_writeLog "Frame Counter: "+STR$(GM_frame_count%)
LM_writeLog "Frame Time: "+STR$(GM_frame_time_ms!)
LM_writeLog "Game Over Status: "+STR$(GM_game_over%)

LM_writeLog "Starting game loop"

xprof_start(3, 39)

GM_run

xprof_stop

LM_writeLog "Game loop ended."
LM_writeLog "Frame Counter: "+STR$(GM_frame_count%)
LM_writeLog "Frame Time: "+STR$(GM_frame_time_ms!)
LM_writeLog "Game Over Status: "+STR$(GM_game_over%)

GM_shutDown

PRINT "GM_isStarted="+STR$(GM_is_Started%)
PRINT "LM_isStarted="+STR$(LM_is_Started%)
PRINT "WM_isStarted="+STR$(WM_is_Started%)

IF num_objLists_allocated%<> 0 THEN
  PRINT "Num ObjLists allocated: "+STR$(num_objLists_allocated%)
  Error "ObjectList leak."
ENDIF

IF num_objs_allocated%<> 0 THEN
  PRINT "Num Objs allocated: "+STR$(num_objs_allocated%)
  Error "Object leak."
ENDIF

LM_writeLog "Test completed."+STR$(LM_is_Started%)

END

FUNCTION TestObject_create%()
  LOCAL obj_id% = Object_create%()
  Object_setType(obj_id%, OBJ_TYPE_TEST%)
  
  Object_registerInterest(obj_id%, EVT_STEP%)

  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Object_destroy(obj_id%, 0)
END SUB

FUNCTION TestObject_eventHandler%(obj_id%, ev%)
 
  LOCAL v%,ii%

  IF (obj_id%=1) AND (Event_getType%(ev%)=EVT_STEP%) THEN
    v% = obj_position%(obj_id%)

    'LM_writeLog "Xpos before ="+STR$(Vector_getX!(v%))

    Vector_setX(v%, INT(Vector_getX!(v%)+1) MOD DM_getHorizontal%())

    'LM_writeLog "Xpos after ="+STR$(Vector_getX!(v%))

    obj_position%(obj_id%) = v%
    
    TestObject_eventHandler% = 1
    EXIT FUNCTION
  ENDIF
    
'  Vector_setXY(v%, RND*DM_getHorizontal%(), RND*DM_getVertical%())
'  Object_setPosition(obj_id%, v%)

'  IF obj_frame_count% > 1000 THEN
'    FOR ii%=0 TO 2
'      Object_setAltitude(moreObjs%(ii%), 3)
'    NEXT ii%
'  ENDIF

'  INC obj_frame_count%  
'  TestObject_eventHandler% = 1
END FUNCTION


SUB TestObject_draw(obj_id%)
  LOCAL position% = obj_position%(obj_id%)
  LOCAL x% = INT(Vector_getX!(position%))
  LOCAL y% = INT(Vector_getY!(position%))
  LOCAL col% = Object_getExtra%(obj_id%)
  
  IF obj_id%=1 THEN
    DM_drawCh(Vector_create%(x%,y%),"X",col%)
  ELSE
    DM_drawCh(Vector_create%(x%,y%),"*",col%)
  ENDIF
END SUB

