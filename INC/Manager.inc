'Requires WorldManager.inc ObjectListIterator.inc, EventTypes.inc, ObjectList.inc

'Per-event-type object lists
DIM mgr_ev_objLists%(MAX_NUM_EVT_TYPES%-1)

'Contrary to the original dragonfly design, Manager here is not a base
'class but a singleton.;

SUB Mgr_startup
  LOCAL ii%
  FOR ii%=0 TO MAX_NUM_EVT_TYPES%-1
    mgr_ev_objLists%(ii%) = ObjList_create%()
  NEXT ii%
END SUB

SUB Mgr_shutDown
  LOCAL ii%
  FOR ii%=0 TO MAX_NUM_EVT_TYPES%-1
    ObjList_destroy(mgr_ev_objLists%(ii%))
  NEXT ii%
END SUB

'Indicate interest in event
SUB mgr_registerInterest(obj_id%, ev_type%)
  ObjList_insert(mgr_ev_objLists%(ev_type%), obj_id%)
  
  LOCAL lid% = mgr_ev_objLists%(ev_type%)
END SUB

'Indicate no longer interested in event
'Returns 0 if success, -1 if object not found
FUNCTION mgr_unregisterInterest%(obj_id%, ev_type%)
  mgr_unregisterInterest% = ObjList_remove%(mgr_ev_objLists%(ev_type%), obj_id%)
END FUNCTION

'Returns count of number of events sent.
FUNCTION onEvent%(ev%)
  LOCAL dummy%, index%
  LOCAL lid% = mgr_ev_objLists%(Event_getType%(ev%))
  LOCAL obj_id%
  
  FOR index%=0 TO objList_numElems%(lid%) - 1 'Yes, this works if numElems = 0
    obj_id% = (objList_list%(index%>>2, lid%) >> (16*(index% AND 3))) AND 65535
    IF (obj_flags%(obj_id%) AND OBJ_FLG_ACT_MSK%) <> 0 THEN
      dummy% = Object_eventHandler%(obj_id%, ev%)
    ENDIF
  NEXT index%
  
  onEvent% = index%
END FUNCTION
