'Requires EventTypes.inc

'Event Base functionality

CONST EVT_TYPE_MSK% =1023
CONST EVT_TYPE_SHFT% =0
CONST EVT_DATA_MSK% = ((-1)<<10)
CONST EVT_DATA_SHFT% = 10

'Event base object is a value type.
FUNCTION Event_create%()
  Event_create% = EVT_UNDEFD%
END FUNCTION

SUB Event_init(ev%)
  ev%=EVT_UNDEFD%
END SUB

'Destroy given event object, releasing all resources used by this object.
SUB Event_destroy(ev%)
  'Poor man's virtual destructor
  LOCAL evt_type% = ev% AND EVT_TYPE_MSK%
  
  SELECT CASE evt_type%
    CASE EVT_COL%
      EvtCol_destroy(ev%)
  END SELECT
END SUB

SUB Event_setType(ev%, type%)
  ev% = (ev% AND (INV EVT_TYPE_MSK%)) OR type%
END SUB

FUNCTION Event_getType%(ev%)
  Event_getType% = ev% AND EVT_TYPE_MSK%
END FUNCTION

SUB Event_setData(ev%, dat%)
  ev% = (ev% AND (INV EVT_DATA_MSK%)) OR (dat%<<EVT_DATA_SHFT%)
END SUB

FUNCTION Event_getData%(ev%)
  Event_getData% = ev% >> EVT_DATA_SHFT%
END FUNCTION
