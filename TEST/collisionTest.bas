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

DIM collision_count% = 0

GM_startUp
LM_writeLog "GM_isStarted="+STR$(GM_is_Started%)
LM_writeLog "LM_isStarted="+STR$(LM_is_Started%)
LM_writeLog "WM_isStarted="+STR$(WM_is_Started%)

DIM testObjs%(3)
testObjs%(0) = TestObject_create%()
testObjs%(1) = TestObject_create%()
testObjs%(2) = TestObject_create%()
testObjs%(3) = TestObject_create%()

Object_setExtra(testObjs%(0), RGB(yellow))
Object_setExtra(testObjs%(1), RGB(blue))
Object_setExtra(testObjs%(2), RGB(yellow))
Object_setExtra(testObjs%(3), RGB(blue))

DIM v%

Vector_setXY(v%, DM_getHorizontal%()*0.25, DM_getVertical%()/2)
obj_position%(testObjs%(0)) = v%
Vector_setXY(v%, 1.0, 0)
Object_setVelocity(testObjs%(0), v%)
Object_setSolidness(testObjs%(0), OBJ_SOLID_HARD%)

Vector_setXY(v%, DM_getHorizontal%()*0.75, DM_getVertical%()/2)
obj_position%(testObjs%(1)) = v%
Vector_setXY(v%, -1.0, 0)
Object_setVelocity(testObjs%(1), v%)
Object_setSolidness(testObjs%(1), OBJ_SOLID_HARD%)

Vector_setXY(v%, DM_getHorizontal%()*0.25, DM_getVertical%()*0.75)
obj_position%(testObjs%(2)) = v%
Vector_setXY(v%, 1.0, 0)
Object_setVelocity(testObjs%(2), v%)
Object_setSolidness(testObjs%(2), OBJ_SOLID_SOFT%)

Vector_setXY(v%, DM_getHorizontal%()*0.75, DM_getVertical%()*0.75)
obj_position%(testObjs%(3)) = v%
Vector_setXY(v%, -1.0, 0)
Object_setVelocity(testObjs%(3), v%)
Object_setSolidness(testObjs%(3), OBJ_SOLID_SOFT%)

LM_writeLog "Frame Counter: "+STR$(GM_frame_count%)
LM_writeLog "Frame Time: "+STR$(GM_frame_time_ms!)
LM_writeLog "Game Over Status: "+STR$(GM_game_over%)

LM_writeLog "Starting game loop"

GM_run

LM_writeLog "Game loop ended. Duration: "+STR$(Clock_split_us%(test_clock%))+"us"
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
  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

FUNCTION TestObject_eventHandler%(obj_id%, ev%)
  TestObject_eventHandler% = 1
  
  LOCAL eType% = Event_getType%(ev%)
  
  IF eType% = EVT_COL% THEN
    INC collision_count%
    LOCAL obj1_id% = EvtCol_getObj1%(ev%)
    LOCAL obj2_id% = EvtCol_getObj2%(ev%)
    LOCAL v% = EvtCol_getPos%(ev%)

    LM_writeLog "Obj "+STR$(obj1_id%)+" collision with obj "+STR$(obj2_id%)+" at position "+STR$(Vector_getX!(v%)) + " " + STR$(Vector_getY!(v%))
    
    Object_setExtra(obj_id%, RGB(red))
    
    IF collision_count% = 100 THEN
      WM_markForDelete(obj_id%)
      
      LOCAL obj3% = TestObject_create%()
      Object_setExtra(obj3%, RGB(yellow))
      Object_setSolidness(obj3%, OBJ_SOLID_SPECTRAL%)

      Vector_setXY(v%, DM_getHorizontal%()*0.25, DM_getVertical%()*0.25)
      obj_position%(obj3%) = v%
      Vector_setXY(v%, 1.0, 0)
      Object_setVelocity(obj3%, v%)

      LOCAL obj4% = TestObject_create%()
      Object_setExtra(obj4%, RGB(blue))
      Object_setSolidness(obj4%, OBJ_SOLID_SPECTRAL%)

      Vector_setXY(v%, DM_getHorizontal%()*0.75, DM_getVertical%()*0.25)
      obj_position%(obj4%) = v%
      Vector_setXY(v%, -1.0, 0)
      Object_setVelocity(obj4%, v%)      
    ENDIF
  ELSE IF eType% = EVT_OUT% THEN
    LM_writeLog "Obj "+STR$(obj_id%)+" out of bounds."
    LOCAL v% = Object_getVelocity%(obj_id%) 
    Object_setVelocity(obj_id%, Vector_scale%(v%,-1))
  ENDIF
END FUNCTION


SUB TestObject_draw(obj_id%)
  LOCAL position% = obj_position%(obj_id%)
  LOCAL x% = INT(Vector_getX!(position%))
  LOCAL y% = INT(Vector_getY!(position%))
  LOCAL col% = Object_getExtra%(obj_id%)

  DM_drawCh(Vector_create%(x%,y%), STR$(obj_id%), col%)  
END SUB

