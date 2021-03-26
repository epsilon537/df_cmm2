
FUNCTION ss_GameStart_create%()
  LOCAL obj_id% = ViewObject_create%()

  Object_setType(obj_id%, OBJ_TYPE_SS_GAMESTART%)
  
  Object_setSprite(obj_id%, "gamestart")

  'Put in center of window
  ViewObject_setLocation(obj_id%, VO_LOC_CENTER_CENTER%)
  
  Object_registerInterest(obj_id%, EVT_KBD%)
  Object_registerInterest(obj_id%, EVT_JOY%)
  
  ss_GameStart_playMusic
  
  ss_GameStart_create% = obj_id%
END FUNCTION

FUNCTION ss_GameStart_eventHandler%(obj_id%, ev%)
  LOCAL eType% = Event_getType%(ev%)

  IF eType% = EVT_KBD% THEN
    LOCAL key% = EVT_KBD_getKey%(ev%) 

    SELECT CASE key%
    CASE P_KEY%
      ss_GameStart_start obj_id%
      ss_GameStart_eventHandler% = 1
      EXIT FUNCTION
    CASE Q_KEY%
      GM_setGameOver(1)
      ss_GameStart_eventHandler% = 1
      EXIT FUNCTION
    END SELECT
  ELSE IF eType% = EVT_JOY% THEN
    IF EVT_JOY_getAction%(ev%) = EVT_JOY_ACT_CLICKED% THEN
      IF EVT_JOY_getButton%(ev%) = EVT_JOY_BUTTON_Z% THEN
        ss_GameStart_start obj_id%
        ss_GameStart_eventHandler% = 1
        EXIT FUNCTION
      ENDIF
    ENDIF
  ENDIF
  
  'If we get here, ignore the event.
  ss_GameStart_eventHandler% = 0
END FUNCTION

SUB ss_GameStart_playMusic
  Sound_play "../sounds/start-music.wav", 1
END SUB

SUB ss_GameStart_start(obj_id%)
  LOCAL dummy% = ss_Hero_create%()
  LOCAL ii%
    
  FOR ii%=0 TO 15
    dummy% = ss_Saucer_create%()
  NEXT ii%
  
  dummy% = ss_Points_create%()
  
  'Count of nukes
  LOCAL nuke_vo% = ViewObject_create%()
  ViewObject_setLocation(nuke_vo%, VO_LOC_TOP_LEFT%)
  ViewObject_setViewString(nuke_vo%, "Nukes")
  ViewObject_setValue(nuke_vo%, 1)
  ViewObject_setColor(nuke_vo%, RGB(yellow))
  ViewObject_setEventTag(nuke_vo%, VIEW_EVENT_TAG_SS_NUKES%)

  'Stop music
  Sound_stop
  
  'When game starts, become inactive.
  Object_setActive obj_id%, 0  
END SUB

