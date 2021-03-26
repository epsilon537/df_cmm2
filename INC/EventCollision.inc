'Requires Event.inc, EventTypes.inc

CONST MAX_NUM_COL_EVENTS% = 1000 'Max. num. objects that can exist in the game.

'Event flags decoding masks and shifts
CONST COL_EVT_FLG_ALLOCATED_MSK% = 1   'Collision Event Object is allocated flag
CONST COL_EVT_FLG_ALLOCATED_SHFT% = 0
CONST COL_EVT_FLG_OBJ1_MSK% = (1023<<1)   'Object 1 mask and shift
CONST COL_EVT_FLG_OBJ1_SHFT% = 1
CONST COL_EVT_FLG_OBJ2_MSK% = (1023<<11)   'Object 2 mask and shift
CONST COL_EVT_FLG_OBJ2_SHFT% = 11

DIM col_position%(MAX_NUM_COL_EVENTS%-1) 'Collision position is a Vector object
'Various collision state values packed into an int. See mask and shift values above.
DIM col_flags%(MAX_NUM_COL_EVENTS%-1) 

'Index into table above used to find the next available (unallocated) object 
DIM col_allocate_index%=0

'Used to track resource leaks
DIM num_cols_allocated%=0

'Allocate a collision event object from the heap and initialize it with given params. Returns an Event object.
FUNCTION EvtCol_create%(obj1_id%, obj2_id%, where%)
  LOCAL ii% = col_allocate_index%
  
  DO
    IF col_flags%(ii%) = 0 THEN 'Free entry found
      'Allocate, set obj. ids.
      col_flags%(ii%) = (obj1_id%<<COL_EVT_FLG_OBJ1_SHFT%) OR (obj2_id%<<COL_EVT_FLG_OBJ2_SHFT%) OR 1
      'Position
      col_position%(ii%) = where%
      'Update index to next hopefully free entry
      col_allocate_index% = ii% + 1
      IF col_allocate_index% >= MAX_NUM_COL_EVENTS% THEN
        col_allocate_index%=0
      ENDIF
    
      'Create base event object
      LOCAL ev% = Event_create%()      
      Event_setType(ev%, EVT_COL%)
      'Set collision event obj.id as event data.
      Event_setData(ev%, ii%)

      INC num_cols_allocated%  
      EvtCol_create% = ev%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= MAX_NUM_COL_EVENTS% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = col_allocate_index%
  
  ERROR "Collision Event Object Heap exhausted!"
END FUNCTION

'Destroy given collision event object, releasing all resources used by this object.
SUB EvtCol_destroy(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eo_id% = Event_getData%(ev%)
  
  IF col_flags%(eo_id%) = 0 THEN
    ERROR "Attempt to free an Collision Event Object that was not allocated."
  ENDIF
  
  col_flags%(eo_id%) = 0
  
  INC num_cols_allocated%, -1  
END SUB

'Returns non-zero if event object is allocated.
FUNCTION EvtCol_isAllocated%(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eo_id% = Event_getData%(ev%)

  EvtCol_isAllocated% = col_flags%(eo_id%)
END FUNCTION

'Set object that caused collision.
SUB EvtCol_setObj1(ev%, obj_id%)
  'Collision event obj id is stored in base event data
  LOCAL eo_id% = Event_getData%(ev%)
  col_flags%(eo_id%) = (col_flags%(eo_id%) AND (INV COL_EVT_FLG_OBJ1_MSK%)) OR (obj_id%<<COL_EVT_FLG_OBJ1_SHFT%)
END SUB

'Return object that cause collision.
FUNCTION EvtCol_getObj1%(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eo_id% = Event_getData%(ev%)
  EvtCol_getObj1% = (col_flags%(eo_id%) AND COL_EVT_FLG_OBJ1_MSK%) >> COL_EVT_FLG_OBJ1_SHFT%
END FUNCTION

'Set object that was collided with.
SUB EvtCol_setObj2(ev%, obj_id%)
  'Collision event obj id is stored in base event data
  LOCAL eo_id% = Event_getData%(ev%)
  col_flags%(eo_id%) = (col_flags%(eo_id%) AND (INV COL_EVT_FLG_OBJ2_MSK%)) OR (obj_id%<<COL_EVT_FLG_OBJ2_SHFT%)
END SUB

'Return object that was collided with.
FUNCTION EvtCol_getObj2%(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eo_id% = Event_getData%(ev%)
  EvtCol_getObj2% = (col_flags%(eo_id%) AND COL_EVT_FLG_OBJ2_MSK%) >> COL_EVT_FLG_OBJ2_SHFT%
END FUNCTION

'Set position of collision
SUB EvtCol_setPos(ev%, where%)
  'Collision event obj id is stored in base event data
  LOCAL eo_id% = Event_getData%(ev%)
  col_position%(eo_id%) = where%
END SUB

'Get position of collision
FUNCTION EvtCol_getPos%(ev%)
  'Collision event obj id is stored in base event data
  LOCAL eo_id% = Event_getData%(ev%)
  EvtCol_getPos% = col_position%(eo_id%)
END FUNCTION

