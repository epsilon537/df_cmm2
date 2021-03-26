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

DM_startUp

'Set world and view size to DM view dimensions
box_horizontal%(wm_boundary%) = DM_getHorizontal%()
box_vertical%(wm_boundary%) = DM_getVertical%()
Box_copy(wm_view%, wm_boundary%)

DIM longstr%(20)
LONGSTRING CLEAR longstr%()
LONGSTRING APPEND longstr%(), "121212"
DIM frame% = Frame_create%(2, 3, longstr%())

IF Frame_getWidth%(frame%) <> 2 THEN
  Error "Frame width incorrect"
ENDIF

IF Frame_getHeight%(frame%) <> 3 THEN
  Error "Frame height incorrect"
ENDIF

DIM longstr2%(20)
LONGSTRING CLEAR longstr2%()

Frame_getLongString(frame%, longstr2%())
IF LCOMPARE(longstr%(), longstr2%()) <> 0 THEN
  ERROR "Frame string incorrect"
ENDIF

DIM v% = Vector_create%(DM_getHorizontal%()/2, DM_getVertical%()/2)
Frame_draw frame%, v%, RGB(BLUE)

Frame_setHeight frame%, 5
Frame_setWidth frame%, 5
LONGSTRING CLEAR longstr%()
LONGSTRING APPEND longstr%(), "1234567890abcdeABCDE54321"

Frame_setLongString frame%, longstr%()

v% = Vector_create%(DM_getHorizontal%()*0.25, DM_getVertical%()/2)
Frame_draw frame%, v%, RGB(YELLOW)

DM_swapBuffers

PRINT "Test Complete"

DO: LOOP

END


