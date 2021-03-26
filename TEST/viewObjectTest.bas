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

box_horizontal%(wm_boundary%) = box_horizontal%(wm_boundary%)*2
box_vertical%(wm_boundary%) = box_vertical%(wm_boundary%)*2
box_corner%(wm_view%) = box_corner%(wm_view%)+Vector_create%(MM.HRES/2, MM.VRES/2)

DIM vo1% = ViewObject_create%()
ViewObject_setViewString(vo1%, "Hello World")

DIM vo2% = ViewObject_create%()
ViewObject_setViewString(vo2%, "Hello World")
ViewObject_setLocation(vo2%, VO_LOC_CENTER_CENTER%)
ViewObject_setBorder(vo2%, 0)

DIM vo3% = ViewObject_create%()
ViewObject_setViewString(vo3%, "Hello World")
ViewObject_setLocation(vo3%, VO_LOC_BOTTOM_LEFT%)
ViewObject_setBorder(vo3%, 1)

DIM vo4% = ViewObject_create%()
ViewObject_setViewString(vo4%, "Hello World")
ViewObject_setLocation(vo4%, VO_LOC_BOTTOM_RIGHT%)
ViewObject_setBorder(vo4%, 1)
ViewObject_setColor(vo4%, RGB(yellow))

DIM vo5% = ViewObject_create%()
ViewObject_setViewString(vo5%, "Hello World")
ViewObject_setLocation(vo5%, VO_LOC_TOP_LEFT%)
ViewObject_setBorder(vo5%, 1)
ViewObject_setColor(vo5%, RGB(yellow))
ViewObject_setDrawValue(vo5%, 0)

DIM vo6% = ViewObject_create%()
ViewObject_setViewString(vo6%, "Hello there!")
ViewObject_setLocation(vo6%, VO_LOC_CENTER_RIGHT%)
ViewObject_setBorder(vo6%, 1)
ViewObject_setColor(vo6%, RGB(yellow))
ViewObject_setDrawValue(vo6%, 1)
ViewObject_setValue(vo6%, 12345678)

IF ViewObject_getViewString$(vo6%) <> "Hello there!") THEN
  ERROR "ViewObject_getViewString error."
ENDIF

IF ViewObject_getLocation%(vo6%) <> VO_LOC_CENTER_RIGHT%) THEN
  ERROR "ViewObject_getLocation error."
ENDIF

IF ViewObject_getBorder%(vo6%) <> 1) THEN
  ERROR "ViewObject_getBorder error."
ENDIF

IF ViewObject_getColor%(vo6%) <> RGB(yellow) THEN
  ERROR "ViewObject_getColor error."
ENDIF

IF ViewObject_getDrawValue%(vo6%) <> 1 THEN
  ERROR "ViewObject_getDrawValue error."
ENDIF

IF ViewObject_getValue%(vo6%) <> 12345678 THEN
  ERROR "ViewObject_getValue error."
ENDIF

GM_run

GM_shutDown

END

