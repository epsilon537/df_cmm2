
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

DIM obj% = TestObject_create%()
DIM vo% = ViewObject_create%()
ViewObject_setViewString(vo%, "Points")
ViewObject_setEventTag(vo%, VIEW_EVENT_TAG_POINTS%)

GM_run

GM_shutDown

END

FUNCTION TestObject_create%()
  LOCAL obj_id% = Object_create%()
  Object_setType(obj_id%, OBJ_TYPE_TEST%)
  Object_registerInterest(obj_id%, EVT_STEP%)
  TestObject_create% = obj_id%
END FUNCTION

SUB TestObject_destroy(obj_id%)
  PRINT "TestObject destroyed: "+STR$(obj_id%)
END SUB

SUB TestObject_draw(obj_id%)
END SUB

FUNCTION TestObject_eventHandler%(obj_id%, ev%)
  IF Event_getType%(ev%) = EVT_STEP% THEN
    LOCAL frameCount% = GM_frame_count%
    LOCAL dummy%
   
    IF frameCount% = 100 THEN
      LOCAL ev1% = EVT_View_create%(VIEW_EVENT_TAG_POINTS%, 10, 1)
      IF EVT_View_getTag%(ev1%) <> VIEW_EVENT_TAG_POINTS% THEN
        ERROR "EVT_View_getTag error."
      ENDIF
      IF EVT_View_getValue%(ev1%) <> 10 THEN
        ERROR "EVT_View_getValue error."
      ENDIF
      IF EVT_View_getDelta%(ev1%) <> 1 THEN
        ERROR "EVT_View_getDelta error."
      ENDIF
      
      dummy% = onEvent%(ev1%)
    ENDIF

    IF frameCount% = 200 THEN
      LOCAL ev2% = EVT_View_create%(VIEW_EVENT_TAG_POINTS%, 10, 1)
      IF EVT_View_getTag%(ev2%) <> VIEW_EVENT_TAG_POINTS% THEN
        ERROR "EVT_View_getTag error."
      ENDIF
      IF EVT_View_getValue%(ev2%) <> 10 THEN
        ERROR "EVT_View_getValue error."
      ENDIF
      IF EVT_View_getDelta%(ev2%) <> 1 THEN
        ERROR "EVT_View_getDelta error."
      ENDIF
      
      dummy% = onEvent%(ev2%)
    ENDIF

    IF frameCount% = 300 THEN
      LOCAL ev3% = EVT_View_create%(VIEW_EVENT_TAG_POINTS%, 10, 1)
      EVT_View_setDelta(ev3%, 0)
      EVT_View_setValue(ev3%, 1000)
      
      IF EVT_View_getTag%(ev3%) <> VIEW_EVENT_TAG_POINTS% THEN
        ERROR "EVT_View_getTag error."
      ENDIF
      IF EVT_View_getValue%(ev3%) <> 1000 THEN
        ERROR "EVT_View_getValue error."
      ENDIF
      IF EVT_View_getDelta%(ev3%) <> 0 THEN
        ERROR "EVT_View_getDelta error."
      ENDIF
      
      dummy% = onEvent%(ev3%)
    ENDIF
    
    IF frameCount% = 400 THEN
      ViewObject_setViewString(vo%, "ValMustNotChange")    
    ENDIF

    IF frameCount% = 500 THEN
      LOCAL ev5% = EVT_View_create%(VIEW_EVENT_TAG_POINTS%, 10, 1)
      EVT_View_setDelta(ev5%, 0)
      EVT_View_setValue(ev5%, 1000)
      EVT_View_setTag(ev5%, VIEW_EVENT_TAG_TEST%)
      
      IF EVT_View_getTag%(ev5%) <> VIEW_EVENT_TAG_TEST% THEN
        ERROR "EVT_View_getTag error."
      ENDIF
  
      dummy% = onEvent%(ev5%)
    ENDIF
  ENDIF
  
  TestObject_eventHandler% = 0
END FUNCTION

