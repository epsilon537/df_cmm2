'Requires Object.inc, ObjectTypes.inc, EventTypes.inc, DisplayManager.inc, ViewEventTags

'General location of ViewObject on screen.
CONST VO_LOC_UNDEFINED% = 0
CONST VO_LOC_TOP_LEFT% = 1
CONST VO_LOC_TOP_CENTER% = 2
CONST VO_LOC_TOP_RIGHT% = 3
CONST VO_LOC_CENTER_LEFT% = 4
CONST VO_LOC_CENTER_CENTER% = 5
CONST VO_LOC_CENTER_RIGHT% = 6
CONST VO_LOC_BOTTOM_LEFT% = 7
CONST VO_LOC_BOTTOM_CENTER% = 8
CONST VO_LOC_BOTTOM_RIGHT% = 9

CONST MAX_NUM_VIEW_OBJECTS% = 100

CONST VO_FLG_VAL_MSK% = &HFFFFFF 'value flag
CONST VO_FLG_VAL_SHFT% = 0
CONST VO_FLG_DRAW_VAL_MSK% = 1<<24 'draw value flag
CONST VO_FLG_DRAW_VAL_SHFT% = 24
CONST VO_FLG_BORDER_MSK% = 1<<25 'draw border flag
CONST VO_FLG_BORDER_SHFT% = 25
CONST VO_FLG_LOC_MSK% = 15 << 26 'location flag
CONST VO_FLG_LOC_SHFT% = 26
CONST VO_FLG_COL_MSK% = &HFFFFFFFF << 32 'color flag
CONST VO_FLG_COL_SHFT% = 32

DIM vo_view_string$(MAX_NUM_VIEW_OBJECTS%-1) LENGTH 40
DIM vo_flags%(MAX_NUM_VIEW_OBJECTS%-1)
DIM vo_flags2%(MAX_NUM_VIEW_OBJECTS%-1) 'Currently we only store the view event tag here.

DIM vo_allocate_index%=0

'Used to track resource leaks
DIM num_vos_allocated%=0

'Allocate an view object from the heap and initialize it. 
'Returns an Object id. Note that ViewObject derives from Object.
FUNCTION ViewObject_create%()
  'First allocate a base Object
  LOCAL obj_id% = Object_create%()
  'Set Type, solidness and altitude
  Object_setType(obj_id%, OBJ_TYPE_VIEW_OBJECT%)
  Object_setSolidness(obj_id%, OBJ_SOLID_SPECTRAL%)
  Object_setAltitude(obj_id%, MAX_ALTITUDE%)
  
  'Now allocate a View Object
  LOCAL ii% = vo_allocate_index%
  
  DO
    IF vo_flags%(ii%) = 0 THEN 'Free entry found
      'Allocate, set default View Object attributes
      vo_flags%(ii%) = VO_FLG_DRAW_VAL_MSK% OR VO_FLG_BORDER_MSK% OR (RGB(WHITE)<<VO_FLG_COL_SHFT%)
      vo_flags2%(ii%) = VIEW_EVENT_TAG_UNDEFINED%
      
      'Update index to next hopefully free entry
      vo_allocate_index% = ii% + 1
      IF vo_allocate_index% >= MAX_NUM_VIEW_OBJECTS% THEN
        vo_allocate_index%=0
      ENDIF
 
      INC num_vos_allocated%  
      
      'Set ViewObject id as extra in base Obect
      Object_setExtra(obj_id%, ii%)
      
      ViewObject_setLocation(obj_id%, VO_LOC_TOP_CENTER%)

      'Register interest in view events.
      Object_registerInterest(obj_id%, EVT_VIEW%)

      ViewObject_create% = obj_id%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= MAX_NUM_VIEW_OBJECTS% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = vo_allocate_index%
  
  ERROR "ViewObject Heap exhausted!"
END FUNCTION

'Destroy given view object, releasing all resources used by this object.
'Called through Object_destroy virtual destructor.
SUB ViewObject_destroy(obj_id%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)

  LM_writeLog("Deleting view obj "+STR$(vo_id%))
  IF vo_flags%(vo_id%) = 0 THEN
    ERROR "Attempt to free a ViewObject that was not allocated. id: "+STR$(vo_id%))
  ENDIF
  
  vo_flags%(vo_id%) = 0
      
  INC num_vos_allocated%, -1  
  
  Object_destroy(obj_id%, 0) 'Invoke base class destructor, non-virtual.
END SUB

'Draw view string and, optionally, value and box
SUB ViewObject_draw(obj_id%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  LOCAL colr% = vo_flags%(vo_id%) >> VO_FLG_COL_SHFT%
  LOCAL world_pos% = viewToWorld%(obj_position%(obj_id%))
  LOCAL s$
  
  IF vo_flags%(vo_id%) AND VO_FLG_DRAW_VAL_MSK% THEN 'Display view string + value
    LOCAL val% = vo_flags%(vo_id%) AND VO_FLG_VAL_MSK%
    s$ = " "+vo_view_string$(vo_id%)+" "+STR$(val%)+" "
    DM_drawString(world_pos%, s$, colr%)
  ELSE
    s$ = " "+vo_view_string$(vo_id%)+" "
    'LM_writeLog("draw: "+s$+" vo_id%="+STR$(vo_id%)+" colr:"+HEX$(colr%)+" obj id:"+STR$(obj_id%))
    DM_drawString(world_pos%, s$, colr%) 'Display view string but no value
  ENDIF

  IF vo_flags%(vo_id%) AND VO_FLG_BORDER_MSK% THEN 'Border around display
    LOCAL boxx% = Vector_getXint%(obj_position%(obj_id%)) - LEN(s$)*MM.INFO(FONTWIDTH)\2
    LOCAL boxy% = Vector_getYint%(obj_position%(obj_id%)) - MM.INFO(FONTHEIGHT)
    LOCAL w% = LEN(s$)*MM.INFO(FONTWIDTH)
    LOCAL h% = MM.INFO(FONTHEIGHT)*2
    BOX boxx%, boxy%, w%, h%, 1,colr%
  ENDIF
END SUB

'Set general location of ViewObject on screen
SUB ViewObject_setLocation(obj_id%, new_location%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  
  'Set position based on location
  SELECT CASE new_location%
    CASE VO_LOC_TOP_LEFT%
      obj_position%(obj_id%) = Vector_create%(MM.HRES*1/6, MM.INFO(FONTHEIGHT))
    CASE VO_LOC_TOP_CENTER%
      obj_position%(obj_id%) = Vector_create%(MM.HRES*3/6, MM.INFO(FONTHEIGHT))    
    CASE VO_LOC_TOP_RIGHT%
      obj_position%(obj_id%) = Vector_create%(MM.HRES*5/6, MM.INFO(FONTHEIGHT))    
    CASE VO_LOC_CENTER_LEFT%
      obj_position%(obj_id%) = Vector_create%(MM.HRES*1/6, MM.VRES/2)    
    CASE VO_LOC_CENTER_CENTER%
      obj_position%(obj_id%) = Vector_create%(MM.HRES*3/6, MM.VRES/2)    
    CASE VO_LOC_CENTER_RIGHT%
      obj_position%(obj_id%) = Vector_create%(MM.HRES*5/6, MM.VRES/2)        
    CASE VO_LOC_BOTTOM_LEFT%
      obj_position%(obj_id%) = Vector_create%(MM.HRES*1/6, MM.VRES-MM.INFO(FONTHEIGHT))        
    CASE VO_LOC_BOTTOM_CENTER%
      obj_position%(obj_id%) = Vector_create%(MM.HRES*3/6, MM.VRES-MM.INFO(FONTHEIGHT))            
    CASE VO_LOC_BOTTOM_RIGHT%
      obj_position%(obj_id%) = Vector_create%(MM.HRES*5/6, MM.VRES-MM.INFO(FONTHEIGHT))                
  END SELECT  
  
  vo_flags%(vo_id%) = (vo_flags%(vo_id%) AND (INV VO_FLG_LOC_MSK%)) OR (new_location% << VO_FLG_LOC_SHFT%)
END SUB

'Get general location of ViewObject on screen
FUNCTION ViewObject_getLocation%(obj_id%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  ViewObject_getLocation% = (vo_flags%(vo_id%) AND VO_FLG_LOC_MSK%) >> VO_FLG_LOC_SHFT%
END FUNCTION

'Set view value
SUB ViewObject_setValue(obj_id%, new_value%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  vo_flags%(vo_id%) = (vo_flags%(vo_id%) AND (INV VO_FLG_VAL_MSK%)) OR new_value%
END SUB

'Get view value
FUNCTION ViewObject_getValue%(obj_id%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  ViewObject_getValue% = vo_flags%(vo_id%) AND VO_FLG_VAL_MSK%
END FUNCTION

'Set view border
SUB ViewObject_setBorder(obj_id%, new_border%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  vo_flags%(vo_id%) = (vo_flags%(vo_id%) AND (INV VO_FLG_BORDER_MSK%)) OR (new_border% << VO_FLG_BORDER_SHFT%)

  'Reset location to account for border setting
  ViewObject_setLocation(obj_id%, ViewObject_getLocation%(obj_id%))
END SUB

'Get view border
FUNCTION ViewObject_getBorder%(obj_id%, new_border%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  ViewObject_getBorder% = (vo_flags%(vo_id%) AND VO_FLG_BORDER_MSK%) >> VO_FLG_BORDER_SHFT%
END FUNCTION

'Set view color
SUB ViewObject_setColor(obj_id%, new_col%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  vo_flags%(vo_id%) = (vo_flags%(vo_id%) AND (INV VO_FLG_COL_MSK%)) OR (new_col%<<VO_FLG_COL_SHFT%)
END SUB

'Get view color
FUNCTION ViewObject_getColor%(obj_id%, new_col%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  ViewObject_getColor% = (vo_flags%(vo_id%) AND VO_FLG_COL_MSK%) >> VO_FLG_COL_SHFT%
END FUNCTION

'Set view display string
SUB ViewObject_setViewString(obj_id%, new_string$)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  vo_view_string$(vo_id%) = new_string$
END SUB

'Get view string
FUNCTION ViewObject_getViewString$(obj_id%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  ViewObject_getViewString$ = vo_view_string$(vo_id%)
END FUNCTION

'Set to draw value with display string
SUB ViewObject_setDrawValue(obj_id%, new_draw_value%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  vo_flags%(vo_id%) = (vo_flags%(vo_id%) AND (INV VO_FLG_DRAW_VAL_MSK%)) OR (new_draw_value% << VO_FLG_DRAW_VAL_SHFT%)
END SUB

'Get draw value flag
FUNCTION ViewObject_getDrawValue%(obj_id%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  ViewObject_getDrawValue% = (vo_flags%(vo_id%) AND VO_FLG_DRAW_VAL_MSK%) >> VO_FLG_DRAW_VAL_SHFT%
END FUNCTION

'Set the view event tag that this object responds to. See ViewEventTags.inc.
SUB ViewObject_setEventTag(obj_id%, tag%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  vo_flags2%(vo_id%) = tag%
END SUB

'Get the view event tag that this object responds to.
FUNCTION ViewObject_getEventTag%(obj_id%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)
  ViewObject_getEventTag% = vo_flags2%(vo_id%)
END FUNCTION

'Handle view event if tag matches the view string.
'Return 0 if ignored, 1 if handled.
FUNCTION ViewObject_eventHandler%(obj_id%, ev%)
  LOCAL vo_id% = Object_getExtra%(obj_id%)

  'See if this is a view event
  IF Event_getType%(ev%) = EVT_VIEW% THEN
    'See if this event is meant for this object.
    IF EVT_View_getTag%(ev%) = vo_flags2%(vo_id%) THEN
      IF EVT_View_getDelta%(ev%) THEN
        LOCAL new_val% = (vo_flags%(vo_id%) + EVT_View_getValue%(ev%)) AND VO_FLG_VAL_MSK%
        vo_flags%(vo_id%) = (vo_flags%(vo_id%) AND (INV VO_FLG_VAL_MSK%)) OR new_val%
      ELSE
        vo_flags%(vo_id%) = (vo_flags%(vo_id%) AND (INV VO_FLG_VAL_MSK%)) OR (EVT_View_getValue%(ev%)<<VO_FLG_VAL_SHFT%)
      ENDIF
      
      'Event is handled
      ViewObject_eventHandler% = 1
    ENDIF
  ENDIF
  
  ViewObject_eventHandler% = 0
END FUNCTION

