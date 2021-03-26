
FUNCTION ss_Bullet_create%(hero_pos%)
  LOCAL obj_id% = Object_create%()
  
  'Link to sprite
  Object_setSprite(obj_id%, "bullet")
  Object_setType(obj_id%, OBJ_TYPE_SS_BULLET%)
  LOCAL p% = hero_pos% + Vector_create%(3*MM.INFO(FONTWIDTH),0)
  obj_position%(obj_id%) = p%
  
  'Bullets move 0.75 space each game loop
  Object_setVelocity(obj_id%, Vector_create%(0.75*MM.INFO(FONTWIDTH),0))  

  'Make bullets soft so can pass through hero.
  Object_setSolidness(obj_id%, OBJ_SOLID_SOFT%)
  
  ss_Bullet_create% = obj_id%
END FUNCTION

FUNCTION ss_Bullet_eventHandler%(obj_id%, ev%)
  LOCAL eType% = Event_getType%(ev%)
  IF eType% = EVT_OUT% THEN
    WM_markForDelete(obj_id%)
    ss_Bullet_eventHandler% = 1
    EXIT FUNCTION
  ELSEIF eType% = EVT_COL% THEN
    ss_Bullet_hit(ev%)
    ss_Bullet_eventHandler% = 1
    EXIT FUNCTION
  ENDIF
  
  ss_Bullet_eventHandler% = 0
END FUNCTION

SUB ss_Bullet_hit(ev%)
  LOCAL obj1_id% = EvtCol_getObj1%(ev%)
  LOCAL obj2_id% = EvtCol_getObj2%(ev%)
  
  IF (Object_getType%(obj1_id%) = OBJ_TYPE_SS_SAUCER%) OR (Object_getType%(obj2_id%) = OBJ_TYPE_SS_SAUCER%) THEN
    WM_markForDelete(obj1_id%)
    WM_markForDelete(obj2_id%)
  ENDIF
END SUB
