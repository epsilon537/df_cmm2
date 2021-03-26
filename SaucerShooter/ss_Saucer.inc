
FUNCTION ss_Saucer_create%()
  LOCAL obj_id% = Object_create%()

  Object_registerInterest(obj_id%, EVT_SS_NUKE%)  
  
  'Link to sprite
  Object_setSprite(obj_id%, "saucer")
  Object_setType(obj_id%, OBJ_TYPE_SS_SAUCER%)

  'Set speed in horizontal direction.
  Object_setVelocity(obj_id%, Vector_create%(-0.75*MM.INFO(FONTWIDTH),0))
  
  ss_Saucer_moveToStart(obj_id%)
  
  ss_Saucer_create% = obj_id%
END FUNCTION

FUNCTION ss_Saucer_eventHandler%(obj_id%, ev%)
  LOCAL eType% = Event_getType%(ev%)
  
  IF eType% = EVT_OUT% THEN
    ss_Saucer_out(obj_id%)
    ss_Saucer_eventHandler% = 1  
  ELSEIF eType% = EVT_COL% THEN
    ss_Saucer_hit(obj_id%, ev%)
    ss_Saucer_eventHandler% = 1  
  ELSEIF eType% = EVT_SS_NUKE% THEN
    ss_Saucer_eventHandler% = 1
    ss_Saucer_nuke(obj_id%)
  ENDIF
  
  ss_Saucer_eventHandler% = 0
END FUNCTION

SUB ss_Saucer_out(obj_id%)
  IF Vector_getXint%(obj_position%(obj_id%)) >= 0 THEN
    EXIT SUB
  ENDIF
  
  ss_Saucer_moveToStart(obj_id%)
  
  'Spawn new Saucer to make the game get harder
  IF RND > 0.5 THEN
    LOCAL dummy% = ss_Saucer_create%()
  ENDIF
END SUB

SUB ss_Saucer_moveToStart(obj_id%)
  STATIC cl% = ObjList_create%()

  LOCAL world_hor% = box_horizontal%(wm_boundary%)
  LOCAL world_ver% = box_vertical%(wm_boundary%)

  'x is off the right side of the window
  LOCAL x! = world_hor% + RND*world_hor% + 3
  LOCAL y! = RND * (world_ver% - 4) + 4
  LOCAL p% = Vector_create%(x!, y!)
  
  'If collision, move right slightly until empty space.
  WM_getCollisions(obj_id%, p%, cl%)
  DO WHILE NOT ObjList_isEmpty%(cl%)
    Vector_setX(p%, Vector_getX!(p%)+2*MM.INFO(FONTWIDTH))
    WM_getCollisions(obj_id%, p%, cl%)
  LOOP

  LOCAL dummy% = WM_moveObject%(obj_id%, p%)
END SUB

SUB ss_Saucer_hit(obj_id%, ev%)
  LOCAL obj1_id% = EvtCol_getObj1%(ev%)
  LOCAL obj2_id% = EvtCol_getObj2%(ev%)

  IF (Object_getType%(obj1_id%) = OBJ_TYPE_SS_SAUCER%) AND (Object_getType%(obj2_id%) = OBJ_TYPE_SS_SAUCER%) THEN
    EXIT SUB
  ENDIF

  IF (Object_getType%(obj1_id%) = OBJ_TYPE_SS_BULLET%) OR (Object_getType%(obj2_id%) = OBJ_TYPE_SS_BULLET%) THEN
    'Create an explosion
    LOCAL expl_id% = ss_Explosion_create%()
    obj_position%(expl_id%) = obj_position%(obj_id%)
    
    'Saucers stay around perpetually
    IF RND > 0.5 THEN
      LOCAL dummy% = ss_Saucer_create%()
    ENDIF
  ENDIF  
  
  IF (Object_getType%(obj1_id%) = OBJ_TYPE_SS_HERO%) OR (Object_getType%(obj2_id%) = OBJ_TYPE_SS_HERO%) THEN
    WM_markForDelete(obj1_id%)
    WM_markForDelete(obj2_id%)
  ENDIF
  
  Sound_play "../sounds/explode.wav", 0
END SUB

SUB ss_Saucer_nuke(obj_id%)
  LOCAL expl_id% = ss_Explosion_create%()
  obj_position%(expl_id%) = obj_position%(obj_id%)

  WM_markForDelete(obj_id%)

  'Saucers stay around perpetually
  LOCAL dummy% = ss_Saucer_create%()  
END SUB

SUB ss_Saucer_destroy(obj_id%)
  'Send view event with points to interested objects
  LOCAL ev% = EVT_View_create%(VIEW_EVENT_TAG_POINTS%, 10, 1)
  LOCAL dummy% = onEvent%(ev%)
  
  Object_destroy(obj_id%, 0)
END SUB
