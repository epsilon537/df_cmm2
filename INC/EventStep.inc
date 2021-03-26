'Requires Event.inc, EventType.inc

SUB EventStep_init(ev%, init_step_count%)
  ev% = EVT_STEP% OR (init_step_count% << EVT_DATA_SHFT%)
END SUB

SUB EventStep_setStepCount(ev%, step_count%)
  ev% = (ev% AND (INV EVT_DATA_MSK%)) OR (step_count%<<EVT_DATA_SHFT%)
END SUB

FUNCTION EventStep_getStepCount%(ev%)
  EventStep_getStepCount% = ev% >> EVT_DATA_SHFT%
END FUNCTION
