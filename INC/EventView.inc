
'Event type is in the first 10 bits
CONST EVT_VIEW_TAG_MSK% = 1023<<10
CONST EVT_VIEW_TAG_SHFT% = 10
CONST EVT_VIEW_VAL_MSK% = &HFFFFFF<<20
CONST EVT_VIEW_VAL_SHFT% = 20
CONST EVT_VIEW_DELTA_MSK% = 1<<44
CONST EVT_VIEW_DELTA_SHFT = 44

'EVT_View is a value type. value% is value associated with event and can be negative.
'delta is a boolean indicating if value is a delta or a replace value.
FUNCTION EVT_View_create%(evt_view_tag%, value%, delta%)
  EVT_View_create% = EVT_VIEW% OR (evt_view_tag%<<EVT_VIEW_TAG_SHFT%) OR ((value%<<EVT_VIEW_VAL_SHFT%) AND EVT_VIEW_VAL_MSK%) OR (delta%<<EVT_VIEW_DELTA_SHFT%)
END FUNCTION

'Set tag to new tag
SUB EVT_View_setTag(ev%, evt_view_tag%)
  ev% = (ev% AND (INV EVT_VIEW_TAG_MSK%)) OR (evt_view_tag%<<EVT_VIEW_TAG_SHFT%)
END SUB

'Get tag
FUNCTION EVT_View_getTag%(ev%)
  EVT_View_getTag% = (ev% AND EVT_VIEW_TAG_MSK%) >> EVT_VIEW_TAG_SHFT%
END FUNCTION

'Set value to new value. Value can be negative.
SUB EVT_View_setValue(ev%, value%)
  ev% = (ev% AND (INV EVT_VIEW_VAL_MSK%)) OR ((value%<<EVT_VIEW_VAL_SHFT%) AND EVT_VIEW_VAL_MSK%)
END SUB

'Get value. Can be negative.
FUNCTION EVT_View_getValue%(ev%)
  'Shift all the way to the left, then all the way to the right for sign extension.
  EVT_View_getValue% = (ev%<<20) >>> 40
END FUNCTION

'Set delta to new value (1 or 0)
SUB EVT_View_setDelta(ev%, delta%)
  ev% = (ev% AND (INV EVT_VIEW_DELTA_MSK%)) OR (delta%<<EVT_VIEW_DELTA_SHFT%)
END SUB

'Get delta
FUNCTION EVT_View_getDelta%(ev%)
  EVT_View_getDelta% = (ev% AND EVT_VIEW_DELTA_MSK%) >> EVT_VIEW_DELTA_SHFT%
END FUNCTION



