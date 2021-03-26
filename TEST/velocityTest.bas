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
LM_writeLog "GM_isStarted="+STR$(GM_is_Started%)
LM_writeLog "LM_isStarted="+STR$(LM_is_Started%)
LM_writeLog "WM_isStarted="+STR$(WM_is_Started%)

DIM testObjs%(3)
testObjs%(0) = TestObject_create%()
testObjs%(1) = TestObject_create%()
testObjs%(2) = TestObject_create%()
testObjs%(3) = TestObject_create%()

Object_setSolidness(testObjs%(0), OBJ_SOLID_SPECTRAL%)
Object_setSolidness(testObjs%(1), OBJ_SOLID_SPECTRAL%)
Object_setSolidness(testObjs%(2), OBJ_SOLID_SPECTRAL%)
Object_setSolidness(testObjs%(3), OBJ_SOLID_SPECTRAL%)

Object_setExtra(testObjs%(0), RGB(yellow))
Object_setExtra(testObjs%(1), RGB(blue))
Object_setExtra(testObjs%(2), RGB(white))
Object_setExtra(testObjs%(3), RGB(green))

DIM v%

Vector_setXY(v%, DM_getHorizontal%()/2, DM_getVertical%()/2)
obj_position%(testObjs%(0)) = v%
Vector_setXY(v%, 1.0, 1.0)
Object_setVelocity(testObjs%(0), v%)

Vector_setXY(v%, DM_getHorizontal%()/2, DM_getVertical%()/2)
obj_position%(testObjs%(1)) =  v%
Vector_setXY(v%, -1.0, -1.0)
Object_setVelocity(testObjs%(1), v%)

Vector_setXY(v%, DM_getHorizontal%()/2, DM_getVertical%()/2)
obj_position%(testObjs%(2)) = v%
Vector_setXY(v%, 1.0, -1.0)
Object_setVelocity(testObjs%(2), v%)

Vector_setXY(v%, DM_getHorizontal%()/2, DM_getVertical%()/2)
obj_position%(testObjs%(3)) = v%
Vector_setXY(v%, -1.0, 1.0)
Object_setVelocity(testObjs%(3), v%)

LM_writeLog "Frame Counter: "+STR$(GM_frame_count%)
LM_writeLog "Frame Time: "+STR$(GM_frame_time_ms!)
LM_writeLog "Game Over Status: "+STR$(GM_game_over%)

LM_writeLog "Starting game loop"

GM_run

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
  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Object_destroy(obj_id%, 0) 'destroy base class object
END SUB

FUNCTION TestObject_eventHandler%(obj_id%, ev%)
  TestObject_eventHandler% = 1
END FUNCTION


SUB TestObject_draw(obj_id%)
  LOCAL position% = obj_position%(obj_id%)
  LOCAL x% = INT(Vector_getX!(position%))
  LOCAL y% = INT(Vector_getY!(position%))
  LOCAL col% = Object_getExtra%(obj_id%)

  DM_drawCh(Vector_create%(x%,y%), STR$(obj_id%), col%)  
END SUB

