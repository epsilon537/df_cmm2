
DIM time_to_live%

FUNCTION ss_GameOver_create%()
  LOCAL obj_id% = ViewObject_create%()

  Object_setType(obj_id%, OBJ_TYPE_SS_GAMEOVER%)
  
  Object_setSprite(obj_id%, "gameover")

  LOCAL anim% = Object_getAnim%(obj_id%)
  LOCAL sprid% = Anim_getSprite%(anim%)
  
  time_to_live% = Sprite_getFrameCount%(sprid%)*Sprite_getSlowdown%(sprid%)
  
  'Put in center of window
  ViewObject_setLocation(obj_id%, VO_LOC_CENTER_CENTER%)
  
  Object_registerInterest(obj_id%, EVT_STEP%)
  
  Sound_play "../sounds/game-over.wav", 0

  ss_GameOver_create% = obj_id%
END FUNCTION


FUNCTION ss_GameOver_eventHandler%(obj_id%, ev%)
  LOCAL eType% = Event_getType%(ev%)

  IF eType% = EVT_STEP% THEN
    ss_GameOver_step obj_id%
    ss_GameOver_eventHandler% = 1
    EXIT FUNCTION
  ENDIF
  
  ss_GameOver_eventHandler% = 0
END FUNCTION

SUB ss_GameOver_step(obj_id%)
  INC time_to_live%, -1
  IF time_to_live% <= 0 THEN
    WM_markForDelete(obj_id%)
  ENDIF
END SUB

SUB ss_GameOver_destroy(obj_id%)
  'Remove Saucers and ViewObjects, re-active GameStart
  LOCAL all_obj_lid% = ObjList_create%()
  WM_getAllObjects(all_obj_lid%, 1)

  LOCAL iter%, cur_obj%
  
  ObjListIter_init(iter%, all_obj_lid%)
  ObjListIter_first(iter%)
    
  DO WHILE NOT ObjListIter_isDone%(iter%)
    cur_obj% = ObjListIter_currentObject%(iter%)
    
    SELECT CASE Object_getType%(cur_obj%)
      CASE OBJ_TYPE_SS_SAUCER%, OBJ_TYPE_VIEW_OBJECT%, OBJ_TYPE_SS_POINTS%
        WM_markForDelete(cur_obj%)
        
      CASE OBJ_TYPE_SS_GAMESTART%
        Object_setActive(cur_obj%, 1)
        'Resume start music
        ss_GameStart_playMusic
    END SELECT
    
    ObjListIter_next(iter%)
  LOOP

  ObjList_destroy(all_obj_lid%)
  
  Object_destroy(obj_id%, 0)
END SUB
