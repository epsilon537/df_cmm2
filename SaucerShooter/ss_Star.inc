
FUNCTION ss_Star_create%()
  LOCAL obj_id% = Object_create%()
  Object_setType(obj_id%, OBJ_TYPE_SS_STAR%)
  Object_setSolidness(obj_id%, OBJ_SOLID_SPECTRAL%)
  
  'Set speed in horizontal direction.
  Object_setVelocity(obj_id%, Vector_create%(-MM.INFO(FONTWIDTH)/(RND*10+1),0))
  Object_setAltitude(obj_id%, 0) 'Stars in background
  
  LOCAL world_hor% = box_horizontal%(wm_boundary%)
  LOCAL world_ver% = box_vertical%(wm_boundary%)

  LOCAL x! = box_horizontal%(wm_boundary%)*RND
  LOCAL y! = box_vertical%(wm_boundary%)*RND
  obj_position%(obj_id%) = Vector_create%(x!, y!)

  ss_Star_create% = obj_id%
END FUNCTION

SUB ss_Star_draw(obj_id%)
  DM_drawCh(obj_position%(obj_id%), ".", RGB(white))
END SUB

FUNCTION ss_Star_eventHandler%(obj_id%, ev%)
  LOCAL eType% = Event_getType%(ev%)
  
  IF eType% = EVT_OUT% THEN
    ss_Star_out obj_id%    
    ss_Star_eventHandler%=1
    EXIT FUNCTION
  ENDIF
  
  ss_Star_eventHandler%=0
END FUNCTION

SUB ss_Star_out(obj_id%)
  'Off the right side of the window
  LOCAL x! = box_horizontal%(wm_boundary%)+MM.INFO(FONTWIDTH)*20*RND
  LOCAL y! = box_vertical%(wm_boundary%)*RND
  obj_position%(obj_id%) = Vector_create%(x!, y!)

  Object_setVelocity(obj_id%, Vector_create%(-MM.INFO(FONTWIDTH)/(RND*10+1),0))
END SUB

