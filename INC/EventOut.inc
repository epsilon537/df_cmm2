'Requires Event.inc, EventTypes.inc

FUNCTION EventOut_create%()
  LOCAL ev% = Event_create%()
  Event_setType(ev%, EVT_OUT%)
  EventOut_create% = ev%
END FUNCTION
