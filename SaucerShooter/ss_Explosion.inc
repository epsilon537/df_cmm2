
FUNCTION ss_Explosion_create%()
  LOCAL obj_id% = Object_create%()
  'Link to sprite
  Object_setSprite(obj_id%, "explosion")
  Object_setType(obj_id%, OBJ_TYPE_SS_EXPLOSION%)
  Object_setSolidness(obj_id%, OBJ_SOLID_SPECTRAL%)
  
  Object_registerInterest(obj_id%, EVT_STEP%)  
  
  'Set time to live as long as sprite length
  LOCAL sprid% = Anim_getSprite%(Object_getAnim%(obj_id%))
  LOCAL ttl% = Sprite_getFrameCount%(sprid%)*Sprite_getSlowdown%(sprid%)
  
  Object_setExtra(obj_id%, ttl%)
  
  ss_Explosion_create% = obj_id%
END FUNCTION

FUNCTION ss_Explosion_eventHandler%(obj_id%, ev%)
  IF Event_getType%(ev%) = EVT_STEP% THEN
    LOCAL ttl% = Object_getExtra%(obj_id%)
    INC ttl%, -1
    IF ttl% <= 0 THEN
      WM_markForDelete(obj_id%)
    ENDIF
    Object_setExtra(obj_id%, ttl%)
    
    ss_Explosion_eventHandler% = 1
    EXIT FUNCTION
  ENDIF
  
  ss_Explosion_eventHandler% = 0
END FUNCTION

