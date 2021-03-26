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

DIM obj1% = Object_create%()
DIM obj2% = Object_create%()

PRINT "Obj.1 id="+STR$(obj1%)
PRINT "Obj.2 id="+STR$(obj2%)

IF Object_isAllocated%(obj1%) = 0 THEN
  ERROR "Object allocate failed."
ENDIF

IF Object_isAllocated%(obj2%) = 0 THEN
  ERROR "Object allocate failed."
ENDIF

Object_setType(obj1%, OBJ_TYPE_TEST%)
PRINT "Obj.1 type="+STR$(Object_getType%(obj1%))
PRINT "Obj.2 type="+STR$(Object_getType%(obj2%))

DIM v0%
Vector_setXY(v0%, 0, 0)
IF v0% <> obj_position%(obj1%) THEN
  ERROR "Object default position failed.
ENDIF
IF v0% <> Object_getVelocity%(obj1%) THEN
  ERROR "Object default velocity failed.
ENDIF

IF Object_getSolidness%(obj1%) <> OBJ_SOLID_HARD% THEN
  ERROR "Default solidness error."
ENDIF

Object_setSolidness(obj1%, OBJ_SOLID_SOFT%)
IF Object_getSolidness%(obj1%) <> OBJ_SOLID_SOFT% THEN
  ERROR "Default solidness error."
ENDIF

Object_setSolidness(obj1%, OBJ_SOLID_SPECTRAL%)
IF Object_getSolidness%(obj1%) <> OBJ_SOLID_SPECTRAL% THEN
  ERROR "Default solidness error."
ENDIF

Object_setAltitude(obj1%, 3)
IF Object_getAltitude%(obj1%) <> 3 THEN
  ERROR "Object altitude error."
ENDIF

DIM v1%
Vector_setXY(v1%, 2.5, 3.5)

obj_position%(obj1%) = v1%
IF v1% <> obj_position%(obj1%) THEN
  ERROR "Object_set/getPosition failed.
ENDIF

DIM v2%
Vector_setXY(v2%, 0.5, 0.1)
Object_setVelocity(obj1%, v2%)
IF v2% <> Object_getVelocity%(obj1%) THEN
  ERROR "Object_set/getVelocity failed.
ENDIF

IF Object_predictPosition%(obj1%) <> (v1%+v2%) THEN
  ERROR "Object_predictPosition error."
ENDIF

Object_destroy(obj1%, 1)
Object_destroy(obj2%, 1)

IF Object_isAllocated%(obj1%) <> 0 THEN
  ERROR "Object free failed."
ENDIF

IF Object_isAllocated%(obj2%) <> 0 THEN
  ERROR "Object free failed."
ENDIF

IF num_objs_allocated%<> 0 THEN
  PRINT "Num Objs allocated: "+STR$(num_objs_allocated%)
  Error "Object leak."
ENDIF

GM_shutDown

PRINT "Test Completed."
END

FUNCTION TestObject_create%()
  LOCAL obj_id% = Object_create%()
  Object_setType(obj_id%, OBJ_TYPE_TEST%)
  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
  Object_destroy(obj_id%, 0) 'Invoke base class destructor.
END SUB

