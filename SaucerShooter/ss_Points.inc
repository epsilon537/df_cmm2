
FUNCTION ss_Points_create%()
  LOCAL obj_id% = ViewObject_create%() 'Points derives from ViewObject
  Object_setType(obj_id%, OBJ_TYPE_SS_POINTS%)
  
  ViewObject_setLocation(obj_id%, VO_LOC_TOP_RIGHT%)
  ViewObject_setViewString(obj_id%, "Points")
  ViewObject_setEventTag(obj_id%, VIEW_EVENT_TAG_POINTS%)
  ViewObject_setColor(obj_id%, RGB(yellow))
  
  Object_registerInterest(obj_id%, EVT_STEP%)
  
  ss_Points_create% = obj_id%
END FUNCTION

FUNCTION ss_Points_eventHandler%(obj_id%, ev%)
  IF Event_getType%(ev%) = EVT_STEP% THEN
    IF EventStep_getStepCount%(ev%) MOD 30 = 0 THEN
      ViewObject_setValue(obj_id%, ViewObject_getValue%(obj_id%) + 1)
      ss_Points_eventHandler% = 1
      EXIT FUNCTION
    ENDIF
  ELSE
    'Defer to parent
    ss_Points_eventHandler% = ViewObject_eventHandler%(obj_id%, ev%)
    EXIT FUNCTION
  ENDIF
  
  ss_Points_eventHandler% = 0
END FUNCTION
