'Requires Object_types.inc, WorldManager.inc, Animation.inc, Sprite.inc, Box.inc, Manager.inc, SceneGraph.inc

CONST MAX_ALTITUDE% = 3

CONST MAX_NUM_OBJECTS% = 1000 'Max. num. objects that can exist in the game.

'Types of object solidness
CONST OBJ_SOLID_HARD% = 0
CONST OBJ_SOLID_SOFT% = 1
CONST OBJ_SOLID_SPECTRAL% = 2

'Object flags decoding masks and shifts
CONST OBJ_FLG_ALLOCATED_MSK% = 1   'Object is allocated flag
CONST OBJ_FLG_ALLOCATED_SHFT% = 0
CONST OBJ_FLG_TYPE_MSK% = (255<<1) 'Object Type
CONST OBJ_FLG_TYPE_SHFT% = 1
CONST OBJ_FLG_ALT_MSK% = (15<<9) 'Object altitude
CONST OBJ_FLG_ALT_SHFT% = 9
CONST OBJ_FLG_SOLID_MSK% = (3<<13) 'Object solidness
CONST OBJ_FLG_SOLID_SHFT% = 13
CONST OBJ_FLG_BOX_MSK% = (1023<<15) 'Object bounding box
CONST OBJ_FLG_BOX_SHFT% = 15
CONST OBJ_FLG_ACT_MSK% = (1<<25) 'Object active flag
CONST OBJ_FLG_ACT_SHFT% = 25
CONST OBJ_FLG_XTR_MSK% = ((&HFFFFFFFF)<<32) 'extra data for sub objects
CONST OBJ_FLG_XTR_SHFT% = 32

DIM obj_velocity%(MAX_NUM_OBJECTS%-1) 'Object velocity is a Vector object
DIM obj_position%(MAX_NUM_OBJECTS%-1) 'Object position is a Vector object
'Various object state values packed into an int. See mask and shift values above.
DIM obj_flags%(MAX_NUM_OBJECTS%-1) 
DIM obj_anim%(MAX_NUM_OBJECTS%-1) 'Animation associated with object.

DIM obj_registered_events%(MAX_NUM_OBJECTS%-1) 'Bitmap of registered events.
'Index into table above used to find the next available (unallocated) object.
DIM obj_allocate_index%=0

'Used to track resource leaks
DIM num_objs_allocated%=0

'Allocate an object from the object heap and initialize it. Returns an object id.
FUNCTION Object_create%()
  LOCAL ii% = obj_allocate_index%
  
  DO
    IF obj_flags%(ii%) = 0 THEN 'Free entry found
      'Allocate, set default type (OBJECT), altitude (1/2 of max), solidness (hard), active
      obj_flags%(ii%) = ((MAX_ALTITUDE%\2)<<OBJ_FLG_ALT_SHFT%) OR (OBJ_TYPE_OBJECT%<<OBJ_FLG_TYPE_SHFT%) OR OBJ_FLG_ACT_MSK% OR 1
      'Empty animation object
      obj_anim%(ii%) = Anim_create%()
      'Default position to (0,0)
      obj_position%(ii%) = 0
      'Default velocity is also (0,0)
      obj_velocity%(ii%) = 0
      'Default no registered events
      obj_registered_events%(ii%) = 0      
      'Default bounding box 1x1 character in size, corner 0,0 (obj. relative)
      LOCAL box_id% = Box_create%(Vector_create%(0,0), MM.INFO(FONTWIDTH), MM.INFO(FONTHEIGHT))
      obj_flags%(ii%) = obj_flags%(ii%) OR (box_id%<<OBJ_FLG_BOX_SHFT%)
      
      'Update index to next hopefully free entry
      obj_allocate_index% = ii% + 1
      IF obj_allocate_index% >= MAX_NUM_OBJECTS% THEN
        obj_allocate_index%=0
      ENDIF
    
      WM_insertObject(ii%) 'Add self to game world.
      
      INC num_objs_allocated%  
      Object_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= MAX_NUM_OBJECTS% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = obj_allocate_index%
  
  ERROR "Object Heap exhausted!"
END FUNCTION

'Destroy given object, releasing all resources used by this object.
SUB Object_destroy(obj_id%, virtual%)
  'LM_writeLog("Deleting obj: "+STR$(obj_id%))
  IF obj_flags%(obj_id%) = 0 THEN
    ERROR "Attempt to free an Object that was not allocated. Obj.id: "+STR$(obj_id%))
  ENDIF

  'Poor man's virtual destructor
  LOCAL obj_type% = (obj_flags%(obj_id%) AND OBJ_FLG_TYPE_MSK%) >> OBJ_FLG_TYPE_SHFT%
  
  IF virtual% THEN
    SELECT CASE obj_type%
      CASE OBJ_TYPE_TEST%
        TestObject_destroy(obj_id%)
        EXIT SUB
      CASE OBJ_TYPE_VIEW_OBJECT%
        ViewObject_destroy(obj_id%)
        EXIT SUB
      CASE OBJ_TYPE_TEXT_ENTRY%
        TextEntry_destroy(obj_id%)
        EXIT SUB
      CASE OBJ_TYPE_SS_SAUCER%
        ss_Saucer_destroy(obj_id%)
        EXIT SUB
      CASE OBJ_TYPE_SS_POINTS%
        ViewObject_destroy(obj_id%)    
        EXIT SUB
      CASE OBJ_TYPE_SS_HERO%
        ss_Hero_destroy(obj_id%)
        EXIT SUB
      CASE OBJ_TYPE_SS_GAMEOVER%
        ss_GameOver_destroy(obj_id%)
        EXIT SUB
    END SELECT
  ENDIF

  WM_removeObject obj_id%

  LOCAL box_id% = (obj_flags%(obj_id%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%
  Box_destroy(box_id%)
    
  obj_flags%(obj_id%) = 0
  
  'Remove event registration.
  LOCAL ii% = (MAX_NUM_EVT_TYPES%-1)
  LOCAL dummy%
  DO WHILE ii% <> 0
    IF (obj_registered_events%(obj_id%) AND (1<<ii%)) <> 0 THEN
      dummy% = mgr_unregisterInterest%(obj_id%, ii%)
    ENDIF
    
    INC ii%, -1
  LOOP
    
  INC num_objs_allocated%, -1  
END SUB

'Sets object type. See Object_types.inc.
SUB Object_setType(obj_id%, obj_type%)
  obj_flags%(obj_id%) = (obj_flags%(obj_id%) AND (INV OBJ_FLG_TYPE_MSK%)) OR (obj_type%<<OBJ_FLG_TYPE_SHFT%)
END SUB

'Gets object type.
FUNCTION Object_getType%(obj_id%)
  Object_getType% = (obj_flags%(obj_id%) AND OBJ_FLG_TYPE_MSK%) >> OBJ_FLG_TYPE_SHFT%
END FUNCTION

'Set object altitude
SUB Object_setAltitude(obj_id%, alt%)
  SG_updateAltitude obj_id%, alt%
  obj_flags%(obj_id%) = (obj_flags%(obj_id%) AND (INV OBJ_FLG_ALT_MSK%)) OR (alt%<<OBJ_FLG_ALT_SHFT%)
END SUB

'Gets object altitude.
FUNCTION Object_getAltitude%(obj_id%)
  Object_getAltitude% = (obj_flags%(obj_id%) AND OBJ_FLG_ALT_MSK%) >> OBJ_FLG_ALT_SHFT%
END FUNCTION

'Set object extra data
SUB Object_setExtra(obj_id%, xtr%)
  obj_flags%(obj_id%) = (obj_flags%(obj_id%) AND (INV OBJ_FLG_XTR_MSK%)) OR (xtr%<<OBJ_FLG_XTR_SHFT%)
END SUB

'Gets object extra data.
FUNCTION Object_getExtra%(obj_id%)
  Object_getExtra% = (obj_flags%(obj_id%) AND OBJ_FLG_XTR_MSK%) >> OBJ_FLG_XTR_SHFT%
END FUNCTION

'Set object active state
SUB Object_setActive(obj_id%, new_active%)
  'Update scene graph
  SG_updateActive obj_id%, new_active%
  
  'Set active value
  obj_flags%(obj_id%) = (obj_flags%(obj_id%) AND (INV OBJ_FLG_ACT_MSK%)) OR (new_active%<<OBJ_FLG_ACT_SHFT%)
END SUB

'Get object active state
FUNCTION Object_isActive%(obj_id%)
  Object_isActive% = (obj_flags%(obj_id%) AND OBJ_FLG_ACT_MSK%) >> OBJ_FLG_ACT_SHFT%
END FUNCTION

'Returns non-zero if object is allocated.
FUNCTION Object_isAllocated%(obj_id%)
  Object_isAllocated% = obj_flags%(obj_id%)
END FUNCTION

'Returns 0 if event ignored, 1 if handled.
FUNCTION Object_eventHandler%(obj_id%, ev%)
  'Case selection as a poor man's replacement for polymorphism
  SELECT CASE (obj_flags%(obj_id%) AND OBJ_FLG_TYPE_MSK%) >> OBJ_FLG_TYPE_SHFT%
    CASE OBJ_TYPE_TEXT_ENTRY%
      Object_eventHandler% = TextEntry_eventHandler%(obj_id%, ev%)    
    CASE OBJ_TYPE_TEST%
      Object_eventHandler% = TestObject_eventHandler%(obj_id%, ev%)
    CASE OBJ_TYPE_TEST2%
      Object_eventHandler% = TestObject_eventHandler%(obj_id%, ev%)
    CASE OBJ_TYPE_SS_BULLET%
      Object_eventHandler% = ss_Bullet_eventHandler%(obj_id%, ev%)
    CASE OBJ_TYPE_SS_HERO%
      Object_eventHandler% = ss_Hero_eventHandler%(obj_id%, ev%)
    CASE OBJ_TYPE_SS_SAUCER%
      Object_eventHandler% = ss_Saucer_eventHandler%(obj_id%, ev%)
    CASE OBJ_TYPE_SS_EXPLOSION%
      Object_eventHandler% = ss_Explosion_eventHandler%(obj_id%, ev%)
    CASE OBJ_TYPE_VIEW_OBJECT%
      Object_eventHandler% = ViewObject_eventHandler%(obj_id%, ev%)    
    CASE OBJ_TYPE_SS_POINTS%
      Object_eventHandler% = ss_Points_eventHandler%(obj_id%, ev%)
    CASE OBJ_TYPE_SS_GAMESTART%
      Object_eventHandler% = ss_GameStart_eventHandler%(obj_id%, ev%)
    CASE OBJ_TYPE_SS_GAMEOVER%
      Object_eventHandler% = ss_GameOver_eventHandler%(obj_id%, ev%)
    CASE OBJ_TYPE_SS_STAR%
      Object_eventHandler% = ss_Star_eventHandler%(obj_id%, ev%)
    CASE ELSE
      Object_eventHandler% = 0      
  END SELECT  
END FUNCTION

'Draw self at position
SUB Object_draw(obj_id%, virtual%)
  LOCAL objType% = (obj_flags%(obj_id%) AND OBJ_FLG_TYPE_MSK%) >> OBJ_FLG_TYPE_SHFT%

  IF virtual% THEN
    'Case selection as a poor man's replacement for polymorphism
    SELECT CASE objType%
      CASE OBJ_TYPE_TEST%
        TestObject_draw(obj_id%)
        EXIT SUB
      CASE OBJ_TYPE_VIEW_OBJECT%
        ViewObject_draw(obj_id%)
        EXIT SUB
      CASE OBJ_TYPE_SS_POINTS%
        ViewObject_draw(obj_id%)
        EXIT SUB
      CASE OBJ_TYPE_TEXT_ENTRY%
        ViewObject_draw(obj_id%)
        EXIT SUB
      CASE OBJ_TYPE_SS_STAR%
        ss_Star_draw(obj_id%)
        EXIT SUB      
    END SELECT  
  ENDIF
  
  'default draw method
  Anim_draw(obj_anim%(obj_id%), obj_position%(obj_id%))  
END SUB

SUB Object_setSolidness(obj_id%, solidness%)
  'Updat scenegraph and solidness
  SG_updateSolidness obj_id%, solidness%
  obj_flags%(obj_id%) = (obj_flags%(obj_id%) AND (INV OBJ_FLG_SOLID_MSK%)) OR (solidness%<<OBJ_FLG_SOLID_SHFT%)
END SUB

FUNCTION Object_getSolidness%(obj_id%)
  Object_getSolidness% = (obj_flags%(obj_id%) AND OBJ_FLG_SOLID_MSK%) >> OBJ_FLG_SOLID_SHFT%
END FUNCTION

'Set object velocity. Velocity is a vector object
SUB Object_setVelocity(obj_id%, v%)
  obj_velocity%(obj_id%) = v%
END SUB

'Get object position.
FUNCTION Object_getVelocity%(obj_id%)
  Object_getVelocity% = obj_velocity%(obj_id%)
END FUNCTION 

FUNCTION Object_predictPosition%(obj_id%)
  Object_predictPosition% = obj_position%(obj_id%) + obj_velocity%(obj_id%)
END FUNCTION

'Set sprite for this object to animate
SUB Object_setSprite(obj_id%, label$)
  LOCAL sprid% = Sprite_find%(label$)
  IF sprid% = -1 THEN
    Error "Sprite "+label$+" not found."
  ENDIF
  
  Anim_setSprite(obj_anim%(obj_id%), sprid%)

  'Set bounding box to match sprite dimensions  
  LOCAL a_box% = Anim_getBox%(obj_anim%(obj_id%))
  LOCAL o_box% = (obj_flags%(obj_id%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%
  Box_copy(o_box%, a_box%)
  box_corner%(o_box%) = Vector_create%(-0.5*box_horizontal%(a_box%), -0.5*box_vertical%(a_box%))
END SUB

'Set animation for this object to a new one
SUB Object_setAnim(obj_id%, anim%)
  obj_anim%(obj_id%) = anim%

  'Set bounding box to match sprite dimensions    
  LOCAL a_box% = Anim_getBox%(anim%)
  LOCAL o_box% = (obj_flags%(obj_id%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%
  Box_copy(o_box%, a_box%)
  box_corner(o_box%) = Vector_create%(-0.5*box_horizontal%(a_box%), -0.5*box_vertical%(a_box%))
END SUB

'Get animation for this object
FUNCTION Object_getAnim%(obj_id%)
  Object_getAnim% = obj_anim%(obj_id%)
END FUNCTION

'Set Object's bounding box, using given box's attributes. Note: Box coordinates are box relative.
SUB Object_setBox(obj_id%, box_id%)
  LOCAL o_box_id% = (obj_flags%(obj_id%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%
  Box_copy(o_box_id%, box_id)
END SUB

'Get Object's bounding box. Note: Box coordinates are box relative.
FUNCTION Object_getBox%(obj_id%)
  Object_getBox% = (obj_flags%(obj_id%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%
END FUNCTION

'Indicate interest in event
SUB Object_registerInterest(obj_id%, ev_type%)
  obj_registered_events%(obj_id%) = obj_registered_events%(obj_id%) OR (1<<ev_type%)
  mgr_registerInterest(obj_id%, ev_type%)
END SUB

'Indicate no longer interested in event
'Returns 0 if success, -1 if object not found
FUNCTION Object_unregisterInterest%(obj_id%, ev_type%)
  obj_registered_events%(obj_id%) = obj_registered_events%(obj_id%) AND (INV (1<<ev_type%))
  Object_unregisterInterest% = mgr_unregisterInterest%(obj_id%, ev_type%)
END FUNCTION

