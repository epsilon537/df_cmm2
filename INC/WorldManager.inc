'Requires ObjectList.inc, ObjectListIterator.inc, Object.inc, Utility.inc, EventCollision.inc, Box.inc, SceneGraph.inc

DIM wm_deletions_lid%
DIM wm_collision_lid%

'Get/Set game world boundary here
DIM wm_boundary% = Box_create%(0,0,0)

'Get/Set player view of world here
DIM wm_view% = Box_create%(0,0,0)

'Object view is following
DIM wm_view_is_following_obj_id% = -1

DIM WM_is_started% = 0

'param: obj_velocity()
CSUB wm_updateInit INTEGER
  00000000
  681B4B01 477061D8 0800031C 
End CSUB
wm_updateInit obj_velocity%(0)

'params: objList_numElems, objList, obj_flags, obj_positions
CSUB getCollisionsInit INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  4C05B430 60E86825 61016820 614A6821 BC306822 47706193 0800031C 
End CSUB
getCollisionsInit objList_numElems%(0), objList_list%(0, 0), obj_flags%(0), obj_position%(0)

'Start all WorldManager services.
SUB WM_startUp
  wm_deletions_lid% = ObjList_create%()
  wm_collision_lid% = ObjList_create%()
  SG_create
  WM_is_started% = 1
END SUB

'Shut down WorldManager services.
SUB WM_shutDown
  LOCAL ol% = ObjList_create%()

  WM_getAllObjects(ol%, 1)

  LOCAL iter%
  ObjListIter_init(iter%, ol%)
  DO WHILE NOT ObjListIter_isDone%(iter%)
    Object_destroy(ObjListIter_currentObject%(iter%), 1)
    ObjListIter_next(iter%)
  LOOP

  ObjList_destroy(ol%)
  
  SG_destroy
  ObjList_destroy(wm_deletions_lid%)
  ObjList_destroy(wm_collision_lid%)
  WM_is_started% = 0
END SUB

'Insert Object into world.
SUB WM_insertObject(obj_id%)
  SG_insertObj obj_id%
END SUB

'Remove Object from world.
SUB WM_removeObject(obj_id%)
  SG_removeObj obj_id%
END SUB

'Return list of all Objects in world.
SUB WM_getAllObjects(all_obj_lid%, inclInActive%)
  IF inclInActive% THEN
    ObjList_copy(active_objects_lid%, all_obj_lid%) 'From to  
    ObjList_append(all_obj_lid%, inactive_objects_lid%) 'To From
  ELSE
    ObjList_copy(active_objects_lid%, all_obj_lid%) 'From to  
  ENDIF
END SUB

'Return list of all Objects in world matching type. Note: list ownership transfers to caller.
'List must be deleted to prevent resource leaks.
FUNCTION WM_objectsOfType%(obj_type%)
  LOCAL lid% = ObjList_create%()
  LOCAL cur_obj_id%, index%=0

  DO WHILE index% < objList_numElems%(active_objects_lid%)
    'cur_obj_id% = ObjListIter_currentObject%(iter%)
    cur_obj_id% = (objList_list%(index%>>2, active_objects_lid%) >> (16*(index% AND 3))) AND 65535

    IF Object_getType%(cur_obj_id%) = obj_type% THEN
      ObjList_insert(lid%, cur_obj_id%)
    ENDIF
    
    INC index%, 1
  LOOP

  WM_objectsOfType% = lid%
END FUNCTION

'Indicate Object is to be deleted at end of current game loop.
SUB WM_markForDelete(obj_id%)
  'Only add once
  LOCAL index%=0

  DO WHILE index% < objList_numElems%(wm_deletions_lid%)
    'IF ObjListIter_currentObject%(iter%) = obj_id% THEN
    IF obj_id% = ((objList_list%(index%>>2, wm_deletions_lid%) >> (16*(index% AND 3))) AND 65535) THEN
      EXIT SUB 'Already marked for delete
    ENDIF
    INC index%
  LOOP

  'Not in list yet, so add:
  ObjList_insert(wm_deletions_lid%, obj_id%)
  
  'LM_writeLog("Marked for delete: "+STR$(obj_id%))
END SUB

'params: active_objects_lid, callout_sub, callout_arg1, callout_arg2
CSUB wm_update_csub INTEGER, STRING, INTEGER, INTEGER
  00000000
  4FF0E92D B0894C35 F44F6800 68247848 68E59206 F800FB08 02C0EB05 E9D26920 
  9000AB00 0F01F1BA 920569A0 F17B9001 69E00200 DB4E9002 0901F101 26009307 
  46412700 9D00464B F00610B2 46180403 02C2EB05 EB020124 58520C01 0520F1C4 
  C004F8DC 3C2040E2 F505FA0C F404FA4C 0205EA42 4322BF58 B2129C02 0CC2EB04 
  FA0F9C01 00D2F882 79E8EA4F E9DC4422 432C4500 E9CDD018 4B111303 9B06681D 
  8900E9C3 3000F8DC C004F8DC 4200E9D2 9B0718E4 020CEB42 605A601C 9B0547A8 
  AB00E9D3 1303E9DD F1473601 45560700 020BEB77 B009DBB7 8FF0E8BD 0800031C 
  08000314 
End CSUB

DIM wm_update_callout_arg1%, wm_update_callout_arg2%
DIM wm_update_dummy%

SUB WM_UPDATE_OBJ_MOVE_CALLOUT
  wm_update_dummy% = WM_moveObject%(wm_update_callout_arg1%, wm_update_callout_arg2%)
END SUB

'Update World. Delete objects marked for deletion.
SUB WM_update
  LOCAL index%=0

  'Careful here: It's possible that an Object destructor adds items to the deletions list
  'in which case the number of elements changes. A FOR loop does not re-evaluate the end
  'condition at every pass so a WHILE loop is necessary here.  
  DO WHILE index% < objList_numElems%(wm_deletions_lid%)
    Object_destroy((objList_list%(index%>>2, wm_deletions_lid%) >> (16*(index% AND 3))) AND 65535, 1)
    INC index%
  LOOP
  
  'Clear deletions list for next update
  objList_numElems%(wm_deletions_lid%) = 0

  wm_update_csub active_objects_lid%, "WM_UPDATE_OBJ_MOVE_CALLOUT"+CHR$(0), wm_update_callout_arg1%, wm_update_callout_arg2%
END SUB

DIM wm_draw_callout_arg%

SUB WM_DRAW_OBJ_DRAW_CALLOUT
  Object_draw(wm_draw_callout_arg%, 1)
END SUB

'parameters: visible_objects_lids, wm_view, callout_sub, callout_arg
CSUB wm_draw_csub INTEGER, INTEGER, STRING, INTEGER
  00000000
  4FF0E92D B0A14C89 68246809 682600CD EB06951B F85605C1 96131031 2319E9CD 
  686B140A 6862920C F1A0921C 921D0208 17DB141A 930B920A 68A26923 69639314 
  17CA9217 F1009315 920D0318 68E2931F 921E69A3 9A1D9316 7148F44F 3F08F852 
  9A1E921D F803FB01 02C3EB02 3400E9D2 2B019218 3402E9CD 0300F174 80B5F2C0 
  25002400 231BE9DD 4500E9CD 0902EB03 0100E9DD 10829914 0303F000 02C2EB01 
  EB02011B F8520608 F1C32008 F1B30120 68760020 F303FA22 F101FA06 0301EA43 
  FA46D402 4303F000 9A15B21B A033F852 F3CA9A16 EB023CC9 E9D202C3 9A131B00 
  00CCEB02 02CCEA4F E9D09212 18522E00 0E0BEB4E 17D11410 462EEA4F 77EEEA4F 
  BC0AE9DD EB77455E DA76020C 9C129A1C 0E04EB02 2E00E9DE 920E18B2 020EEB47 
  E9DD920F E9DD670A 42B4450E 0207EB75 2701BFAC E9DD2700 4558BC0C 020CEB71 
  9A17DA21 44149C12 2600E9D4 450CE9DD 92101882 0206EB41 E9DD9211 42A11210 
  DB2741AA B21AB337 4931981A 680917D3 2300E9C0 1C589B19 9B184788 3400E9D3 
  3402E9CD 9A17E021 9C1B4665 465C1916 2600E9D6 0202EB1B EB4C9206 92070206 
  BC06E9DD EB7C4583 DADB0201 BF08428D D0C64284 72FFF40A 22009208 E9DD9209 
  2A001208 2902BF08 9B00D0CD 93003301 F1439B01 93010300 1200E9DD 3402E9DD 
  EB724299 F6FF0304 9B1FAF53 42939A1D AF31F47F E8BDB021 F8D98FF0 F8D92000 
  EB1B4004 92040202 0204EB4C E9DD9205 42B44504 0207EB75 2701DB01 45BCE78B 
  45B3BF08 AF72F43F E7842700 0800031C 08000314 
END CSUB

'Draw all objects
SUB WM_draw
  wm_draw_csub visible_objects_lid%(0), wm_view%, "WM_DRAW_OBJ_DRAW_CALLOUT"+CHR$(0), wm_draw_callout_arg%
END SUB

'LOCALS made global for performance
DIM wm_move_orig_box% = Box_create%(0,0,0), wm_move_new_box% = Box_create%(0,0,0)
DIM wm_move_orig_box_intersect%, wm_move_new_box_intersect%, wm_boxRel%
DIM wm_dummy%

'Move object
'If collision with solid; send collision events
'If no collision with solid, move ok, else don't move
'If object is spectral, move ok.
'Return 0 if move ok, else -1
FUNCTION WM_moveObject%(obj_id%, where%)
  'Only solid objects can collide
  'IF Object_getSolidness%(obj_id%) <> OBJ_SOLID_SPECTRAL% THEN
  IF (obj_flags%(obj_id%) AND OBJ_FLG_SOLID_MSK%) <> OBJ_SOLID_SPECTRAL%<<OBJ_FLG_SOLID_SHFT% THEN
    'Get collisions

    'Clear given object list
    objList_numElems%(wm_collision_lid%) = 0
  
    getCollisions obj_id%, where%, solid_objects_lid%, wm_collision_lid%
    IF wm_collision_lid%=-1 THEN: ERROR "Collision list is full.": ENDIF
    
    IF objList_numElems%(wm_collision_lid%) <> 0 THEN
      LOCAL do_move% = 1, ev%, currentObject%, index%=0
    
      DO WHILE index% < objList_numElems%(wm_collision_lid%)
        currentObject% = (objList_list%(index%>>2, wm_collision_lid%) >> (16*(index% AND 3))) AND 65535
    
        'Create collision event
        ev% = EvtCol_create%(obj_id%, currentObject%, where%)

        'Send to both objects
        wm_dummy% = Object_eventHandler%(obj_id%, ev%)
        wm_dummy% = Object_eventHandler%(currentObject%, ev%)

        'Destroy collision event
        Event_destroy(ev%)
        
        'If both HARD, then can't move
        IF ((obj_flags%(obj_id%) AND OBJ_FLG_SOLID_MSK%) = OBJ_SOLID_HARD%<<OBJ_FLG_SOLID_SHFT%) AND ((obj_flags%(currentObject%) AND OBJ_FLG_SOLID_MSK%) = OBJ_SOLID_HARD%<<OBJ_FLG_SOLID_SHFT%) THEN
          do_move% = 0
        ENDIF
        
        INC index%
      LOOP 'End iteration
      
      IF do_move%=0 THEN
        WM_moveObject%=-1 'Move not allowed.
        EXIT FUNCTION
      ENDIF
    ENDIF 'No collisions    
  ENDIF 'Object not solid
        
  'No collisions between HARD objects, so can move.
    
  'getWorldBox(obj_id%, orig_box%) 'original bounding box
  wm_boxRel% = (obj_flags%(obj_id%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%
  box_corner%(wm_move_orig_box%) = box_corner%(wm_boxRel%) + obj_position%(obj_id%) 
  box_horizontal%(wm_move_orig_box%) = box_horizontal%(wm_boxRel%)
  box_vertical%(wm_move_orig_box%) = box_vertical%(wm_boxRel%)

  obj_position%(obj_id%) = where% 'move object
  
  'getWorldBox(obj_id%, new_box%) 'new bounding box
  box_corner%(wm_move_new_box%) = box_corner%(wm_boxRel%) + where%
  box_horizontal%(wm_move_new_box%) = box_horizontal%(wm_boxRel%): box_vertical%(wm_move_new_box%) = box_vertical%(wm_boxRel%)

  'If was in bounds but not anymore...
  boxIntersectBox(wm_move_orig_box%, wm_boundary%, wm_move_orig_box_intersect%)
  boxIntersectBox(wm_move_new_box%, wm_boundary%, wm_move_new_box_intersect%)

  IF wm_move_orig_box_intersect% AND (NOT wm_move_new_box_intersect%) THEN
    wm_dummy% = Object_eventHandler%(obj_id%, EventOut_create%())
  ENDIF

  'IF view is following this object, adjust view.
  IF wm_view_is_following_obj_id% = obj_id% THEN
    WM_setViewPosition(obj_position%(obj_id%))
  ENDIF
  
  WM_moveObject%=0
END FUNCTION

'params: obj_id, where, solid_objects_lid, collisions_lid
CSUB getCollisions INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  4FF0E92D B09D4CA6 F44F469C 68247E48 6826931B 961168A3 68E69319 68676813 
  B014F8D4 2000F8DC FC03FB0E A010F8D4 03C3EB06 FB0E69A4 680DFE02 97189412 
  9F196849 E068F8CD 8900E9D0 3400E9D3 E9CD9811 4644340C F85B00E3 F3C33003 
  EB0033C9 E9D404C3 E9CD0400 1945890E EB419818 EB000404 EB0700C3 142F03C3 
  68406801 970617ED 17E41427 94059704 681D9507 E9DD685B 18797804 EB489114 
  E9DD0100 91157806 91161979 0103EB48 340CE9DD 2B019117 0300F174 80C2F2C0 
  06C2EB06 22002300 A00CF8CD 9C039613 F0021090 EB040103 010900C0 050CEB00 
  E00CF850 0020F1C1 0420F1B1 FA2E686D FA05FE01 EA4EF000 D4030E00 F404FA45 
  0E04EA4E F18EFA1F 468E9100 91012100 0100E9DD 450EE9DD BF0842A9 F00042A0 
  99128087 403EF85B 08CEEB01 34C9F3C4 5600E9D8 08C4EA4F 9E119610 09C4EB06 
  E9D99E10 EB144A00 EB4A0905 EA4F0A06 EA4F4429 EA4F75E9 EA4F402A E9DD71EA 
  45819A04 0601EB7A 9E18DB7C 0A08EB06 9A00E9DA 0609EB10 EB419608 9609060A 
  6708E9DD 9A04E9DD EB77454E DB69060A E9DD2101 45A19A06 0005EB7A 9819DB55 
  E9D84480 18200800 EB45900A 900B0008 670AE9DD EB77454E DB46000A D0372900 
  F2409913 2500148F 0100E9D1 EB754284 DB5B0401 46052800 0400F171 1CC5DA00 
  0003F000 010108AD EB00981A F64F05C5 FA0E70FF 4088FE01 46769903 0A05EB01 
  594943C0 F8DA17F7 EA004004 17C10801 EA48400C 9903000E 5148433C 4004F8CA 
  68219C13 31016860 0000F140 60606021 E9DD3201 F143010C 42990300 4290BF08 
  AF45F47F E8BDB01D E9DD8FF0 42B46706 0007EB75 E9DDDBEC 42A66716 0005EB77 
  E7E5DAAC 9A04E9DD EB714548 E9DD060A BFAC6714 0901F04F 0900F04F EB774286 
  F0090101 BFB80101 E7822100 32FFF04F 33FFF04F E9C1991B B01D2300 8FF0E8BD 
  0800031C 
END CSUB

'Populate CL with list of objects collided with, if object were to move to 'where'.
'Collision only with solid objects.
'Does not consider if object itself is solid or not.
SUB WM_getCollisions(obj_id%, where%, cl%)
  'Clear given object list
  objList_numElems%(cl%) = 0

  LOCAL clLocal% = cl%
  getCollisions obj_id%, where%, solid_objects_lid%, clLocal%
  IF clLocal%=-1 THEN: ERROR "ObjList id="+STR$(clLocal%)+" is full.": ENDIF  
END SUB

'Set view to center window on position view_pos
'View edge will not go beyond world boundary
'view_pos is a vector.
SUB WM_setViewPosition(view_pos%)
  'Make sure horizontal not out of world position
  LOCAL view_hor% = box_horizontal%(wm_view%)
  LOCAL bound_hor% = box_horizontal%(wm_boundary%)
  LOCAL x% = (view_pos%>>>48) - (view_hor%\2) 'Vector_getXint%(view_pos%) - view_hor%/2
  IF x% + view_hor% > bound_hor% THEN
    x% = bound_hor% - view_hor%
  ENDIF
  IF x% < 0 THEN
    x% = 0
  ENDIF
  
  'Make sure vertical not out of world position
  LOCAL view_ver% = box_vertical%(wm_view%)
  LOCAL bound_ver% = box_vertical%(wm_boundary%)
  'Parenthesis needed here!
  LOCAL y% = ((view_pos%<<32)>>>48) - (view_ver%\2) 'Vector_getYint%(view_pos%) - view_hor%/2
  IF y% + view_ver% > bound_ver% THEN
    y% = bound_ver% - view_ver%
  ENDIF
  IF y% < 0 THEN
    y% = 0
  ENDIF
  
  'Set view
  box_corner%(wm_view%) = Vector_create%(x%, y%)
END SUB

'Set view to center window on Object
'Set to -1 to stop following.
SUB WM_setViewFollowing(new_view_following_obj_id%)
  wm_view_is_following_obj_id% = new_view_following_obj_id%

  IF new_view_following_obj_id% = -1 THEN
    EXIT SUB
  ENDIF
  
  WM_setViewPosition(obj_position%(new_view_following_obj_id%))
END SUB

