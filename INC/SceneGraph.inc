'Requires ObjectList.inc, Object.inc

'SceneGraph is a singleton

DIM active_objects_lid% 'Active objects list id
DIM inactive_objects_lid% 'Inactive objects list id
DIM solid_objects_lid%  'Solid objectd list id
DIM visible_objects_lid%(MAX_ALTITUDE%) 'Visible objects per altitude

SUB SG_create
  active_objects_lid% = ObjList_create%()
  inactive_objects_lid% = ObjList_create%()
  solid_objects_lid% = ObjList_create%()
    
  LOCAL ii%
  FOR ii%=0 TO MAX_ALTITUDE%
    visible_objects_lid%(ii%) = ObjList_create%()
  NEXT ii%
END SUB

SUB SG_destroy
  ObjList_destroy(active_objects_lid%)
  ObjList_destroy(inactive_objects_lid%)
  ObjList_destroy(solid_objects_lid%)
  
  LOCAL ii%
  FOR ii%=0 TO MAX_ALTITUDE%
    ObjList_destroy visible_objects_lid%(ii%)
  NEXT ii%
END SUB

'Insert Object into SceneGraph.
SUB SG_insertObj(obj_id%)
  'Add to active or inactive list
  IF Object_isActive%(obj_id%) THEN
    ObjList_insert active_objects_lid%, obj_id%
  ELSE
    ObjList_insert inactive_objects_lid%, obj_id%
  ENDIF
  
  'If solid, add to solid objects list.
  IF Object_getSolidness%(obj_id%) <> OBJ_SOLID_SPECTRAL% THEN
    ObjList_insert solid_objects_lid%, obj_id%
  ENDIF
  
  'Add to visible objects at right altitude.
  ObjList_insert visible_objects_lid%(Object_getAltitude%(obj_id%)), obj_id%
END SUB

'Remove Object from SceneGraph.
SUB SG_removeObj(obj_id%)
  'Remove from active or inactive list
  IF Object_isActive%(obj_id%) THEN
    IF ObjList_remove%(active_objects_lid%, obj_id%) = -1 THEN
      ERROR "SG_removeObj: Failed to remove object "+STR$(obj_id%)+" from SceneGraph active objects list."
    ENDIF
  ELSE
    IF ObjList_remove%(inactive_objects_lid%, obj_id%) = -1 THEN
      ERROR "SG_removeObj: Failed to remove object "+STR$(obj_id%)+" from SceneGraph inactive objects list."
    ENDIF
  ENDIF
  
  'If solid, remove from solid objects list.
  IF Object_getSolidness%(obj_id%) <> OBJ_SOLID_SPECTRAL% THEN
    IF ObjList_remove%(solid_objects_lid%, obj_id%) = -1 THEN
      ERROR "SG_removeObj: Failed to remove object "+STR$(obj_id%)+" from SceneGraph solid objects list."
    ENDIF
  ENDIF
  
  'Remove from visible objects at right altitude.
  IF ObjList_remove%(visible_objects_lid%(Object_getAltitude%(obj_id%)), obj_id%) = -1 THEN
    ERROR "SG_removeObj: Failed to remove object "+STR$(obj_id%)+" from SceneGraph visible objects list."
  ENDIF
END SUB

'Re-position Object in SceneGraph to new altitude.
SUB SG_updateAltitude(obj_id%, new_alt%)
  'Remove from old altitude.
  IF ObjList_remove%(visible_objects_lid%(Object_getAltitude%(obj_id%)), obj_id%) = -1 THEN
    ERROR "Failed to remove object from SceneGraph visible objects list"
  ENDIF

  'Add to new altitude.
  ObjList_insert visible_objects_lid%(new_alt%), obj_id%    
END SUB

'Re-position Object in SceneGraph to new solidness.
SUB SG_updateSolidness(obj_id%, new_solidness%)
  'If was solid, remove from solid objects list
  IF Object_getSolidness%(obj_id%) <> OBJ_SOLID_SPECTRAL% THEN
    IF ObjList_remove%(solid_objects_lid%, obj_id%) = -1 THEN
      ERROR "Failed to remove object from SceneGraph solid objects list"
    ENDIF
  ENDIF

  'If solid, add to solid objects list
  IF new_solidness% <> OBJ_SOLID_SPECTRAL% THEN
    ObjList_insert solid_objects_lid%, obj_id%
  ENDIF
END SUB

'Re-position Object in SceneGraph to new activeness.
SUB SG_updateActive(obj_id%, new_active%)
  'If activeness unchanged, nothing to do.
  LOCAL isActive% = Object_isActive%(obj_id%)
  
  IF isActive% = new_active% THEN
    EXIT SUB
  ENDIF
  
  'If was active, then now inactive, so remove from lists.
  IF isActive% THEN
    IF ObjList_remove%(active_objects_lid%, obj_id%) = -1 THEN
      ERROR "Failed to remove object from SceneGraph active objects list"
    ENDIF
    
    IF ObjList_remove%(visible_objects_lid%(Object_getAltitude%(obj_id%)), obj_id%) = -1 THEN
      ERROR "Failed to remove object from SceneGraph visible objects list"
    ENDIF
    
    IF Object_getSolidness%(obj_id%) <> OBJ_SOLID_SPECTRAL% THEN
      IF ObjList_remove%(solid_objects_lid%, obj_id%) = -1 THEN
        ERROR "Failed to remove object from SceneGraph solid objects list"
      ENDIF
    ENDIF    
    
    'Add to inacive list.
    ObjList_insert inactive_objects_lid%, obj_id% 
  
  ELSE 'Was inactive, so add to lists
    ObjList_insert active_objects_lid%, obj_id%'

    'If solid, add to solid objects list.
    IF Object_getSolidness%(obj_id%) <> OBJ_SOLID_SPECTRAL% THEN
      ObjList_insert solid_objects_lid%, obj_id%
    ENDIF

    'Add to visible objects at right altitude.
    ObjList_insert visible_objects_lid%(Object_getAltitude%(obj_id%)), obj_id%

    'Remove from inactive list.
    IF ObjList_remove%(inactive_objects_lid%, obj_id%) = -1 THEN
      ERROR "Failed to remove object from SceneGraph inactive objects list"
    ENDIF      
  ENDIF
END SUB

'Clear all objects from the SceneGraph
SUB SG_Clear
  ObjList_clear active_objects_lid%
  ObjList_clear inactive_objects_lid%
  ObjList_clear solid_objects_lid%

  LOCAL ii%
  FOR ii%=0 TO MAX_ALTITUDE%
    ObjList_clear visible_objects_lid%(ii%)
  NEXT ii%  
END SUB

