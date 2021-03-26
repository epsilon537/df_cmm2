'Hero object flags decoding masks and shifts
CONST HERO_FLG_MOVE_SLOWDOWN_MSK% = 63<<24
CONST HERO_FLG_MOVE_SLOWDOWN_SHFT% = 24
CONST HERO_FLG_MOVE_COUNTDOWN_MSK% = 63<<18
CONST HERO_FLG_MOVE_COUNTDOWN_SHFT% = 18
CONST HERO_FLG_FIRE_SLOWDOWN_MSK% = 63<<12
CONST HERO_FLG_FIRE_SLOWDOWN_SHFT% = 12
CONST HERO_FLG_FIRE_COUNTDOWN_MSK% = 63<<6
CONST HERO_FLG_FIRE_COUNTDOWN_SHFT% = 6
CONST HERO_FLG_NUKE_COUNT_MSK% = 63
CONST HERO_FLG_NUKE_COUNT_SHFT% = 0

FUNCTION ss_Hero_create%()
  LOCAL obj_id% = Object_create%()
  
  'Link to sprite
  Object_setSprite(obj_id%, "ship")
  Object_setType(obj_id%, OBJ_TYPE_SS_HERO%)

  LOCAL p% = Vector_create%(7*MM.INFO(FONTWIDTH), Vector_getY!(box_corner%(wm_view%)) + box_vertical%(wm_view%)/2)
  obj_position%(obj_id%) = p%
  
  'Set slowdown, fire, nuke attributes
  LOCAL xtra% = (2<<HERO_FLG_MOVE_SLOWDOWN_SHFT%) OR (2<<HERO_FLG_MOVE_COUNTDOWN_SHFT%)
  xtra% = xtra% OR (15<<HERO_FLG_FIRE_SLOWDOWN_SHFT%) OR (15<<HERO_FLG_FIRE_COUNTDOWN_SHFT%)
  xtra% = xtra% OR 1
  
  Object_setExtra(obj_id%, xtra%)

  Object_registerInterest(obj_id%, EVT_KBD%)
  Object_registerInterest(obj_id%, EVT_STEP%)
  Object_registerInterest(obj_id%, EVT_JOY%)
  
  'WM_setViewFollowing(obj_id%)
  
  ss_Hero_create% = obj_id%
END FUNCTION

SUB ss_Hero_destroy(obj_id%)
  'Create GameOver object
  LOCAL dummy% = ss_GameOver_create%()
  
  dm_shake_severity%=20
  dm_shake_duration%=20

  Object_destroy(obj_id%, 0)
END SUB

FUNCTION ss_Hero_eventHandler%(obj_id%, ev%)
  LOCAL eType% = Event_getType%(ev%)

  SELECT CASE eType%
    CASE EVT_KBD%
      ss_Hero_eventHandler% = 1
      ss_Hero_kbd(obj_id%, ev%)
      EXIT FUNCTION
    CASE EVT_JOY%
      ss_Hero_eventHandler% = 1
      ss_Hero_joy(obj_id%, ev%)
      EXIT FUNCTION
    CASE EVT_STEP%
      ss_Hero_step(obj_id%)
      ss_Hero_eventHandler% = 1
  END SELECT
  
  ss_Hero_eventHandler% = 0
END FUNCTION

SUB ss_Hero_joy(obj_id%, ev%)
  LOCAL action% = EVT_JOY_getAction%(ev%)
  
  SELECT CASE action%
    CASE EVT_JOY_ACT_CLICKED%
      IF EVT_JOY_getButton%(ev%) = EVT_JOY_BUTTON_Z% THEN
        ss_Hero_fire(obj_id%)
      ELSE
        ss_Hero_nuke(obj_id%)
      ENDIF
    CASE EVT_JOY_ACT_MOVED%
      LOCAL position% = EVT_JOY_getPosition%(ev%)
      LOCAL ypos! = Vector_getY!(position%)
      IF ypos! > 0 THEN
        ss_Hero_move(obj_id%, -MM.INFO(FONTHEIGHT))
      ELSE IF ypos! < 0 THEN
        ss_Hero_move(obj_id%, MM.INFO(FONTHEIGHT))
      ENDIF
  END SELECT
END SUB

SUB ss_Hero_kbd(obj_id%, ev%)
  LOCAL key% = EVT_KBD_getKey%(ev%) 

  SELECT CASE key%
  CASE Q_KEY%
    IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
      WM_markForDelete(obj_id%)
    ENDIF
  CASE W_KEY%
    IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_DOWN% THEN
      ss_Hero_move(obj_id%, -MM.INFO(FONTHEIGHT))
    ENDIF  
  CASE S_KEY%
    IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_DOWN% THEN
      ss_Hero_move(obj_id%, MM.INFO(FONTHEIGHT))
    ENDIF  
  CASE SPC_KEY% 
    IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
      ss_Hero_nuke(obj_id%)
    ENDIF  
  CASE ENTER_KEY%
    IF EVT_KBD_getAction%(ev%) = EVT_KBD_ACT_KEY_PRESSED% THEN
      ss_Hero_fire(obj_id%)
    ENDIF  
  END SELECT
END SUB

'Move up or down
SUB ss_Hero_move(obj_id%, dy%)
  LOCAL xtra% = Object_getExtra%(obj_id%)
  
  LOCAL move_countdown% = (xtra% AND HERO_FLG_MOVE_COUNTDOWN_MSK%)>>HERO_FLG_MOVE_COUNTDOWN_SHFT%
  IF move_countdown% > 0 THEN
    EXIT SUB
  ENDIF

  LOCAL move_slowdown% = (xtra% AND HERO_FLG_MOVE_SLOWDOWN_MSK%)>>HERO_FLG_MOVE_SLOWDOWN_SHFT%
  move_countdown% = move_slowdown%
  xtra% = xtra% AND (INV HERO_FLG_MOVE_COUNTDOWN_MSK%) 
  xtra% = xtra% OR (move_countdown%<<HERO_FLG_MOVE_COUNTDOWN_SHFT%)
  Object_setExtra(obj_id%, xtra%)

  'If stays on window, allow move
  LOCAL dyVector% = Vector_create%(0, dy%)
  LOCAL new_pos% = obj_position%(obj_id%) + dyVector%
  LOCAL new_posY% = Vector_getYint%(new_pos%)
  
  IF (new_posY%>3) AND (new_posY% < box_vertical%(wm_boundary%)-1) THEN
    LOCAL dummy%=WM_moveObject%(obj_id%, new_pos%)
  ENDIF
  
  'Adjust view
  'INC box_corner%(wm_view%), dyVector%
END SUB

'Decrease rate restriction counters.
SUB ss_Hero_step(obj_id%)
  'Move countdown
  LOCAL xtra% = Object_getExtra%(obj_id%)  
  LOCAL move_countdown% = (xtra% AND HERO_FLG_MOVE_COUNTDOWN_MSK%)>>HERO_FLG_MOVE_COUNTDOWN_SHFT%
  INC move_countdown%, -1
  IF move_countdown% < 0 THEN
    move_countdown% = 0
  ENDIF
  
  'Fire countdown
  LOCAL fire_countdown% = (xtra% AND HERO_FLG_FIRE_COUNTDOWN_MSK%)>>HERO_FLG_FIRE_COUNTDOWN_SHFT%
  INC fire_countdown%, -1
  IF fire_countdown% < 0 THEN
    fire_countdown% = 0
  ENDIF

  xtra% = xtra% AND (INV HERO_FLG_MOVE_COUNTDOWN_MSK%) AND (INV HERO_FLG_FIRE_COUNTDOWN_MSK%)
  xtra% = xtra% OR (move_countdown%<<HERO_FLG_MOVE_COUNTDOWN_SHFT%) OR (fire_countdown%<<HERO_FLG_FIRE_COUNTDOWN_SHFT%)
  Object_setExtra(obj_id%, xtra%)
END SUB

SUB ss_Hero_nuke(obj_id%)
  LOCAL xtra% = Object_getExtra%(obj_id%)
  
  LOCAL nuke_count% = (xtra% AND HERO_FLG_NUKE_COUNT_MSK%)>>HERO_FLG_NUKE_COUNT_SHFT%

  'Check if any nukes left
  IF nuke_count% = 0 THEN
    EXIT SUB
  ENDIF
  
  INC nuke_count%, -1

  'Create nuke event and send to interested Objects
  LOCAL ev% = Event_create%()
  Event_setType(ev%, EVT_SS_NUKE%)
  LOCAL dummy% = onEvent%(ev%)

  'Send view event with nukes to interested Objects
  ev% = EVT_View_create%(VIEW_EVENT_TAG_SS_NUKES%, -1, 1)
  dummy% = onEvent%(ev%)
    
  xtra% = xtra% AND (INV HERO_FLG_NUKE_COUNT_MSK%)
  xtra% = xtra% OR nuke_count%
  Object_setExtra(obj_id%, xtra%)
  
  Sound_play "../sounds/nuke.wav", 0
END SUB

SUB ss_Hero_fire(obj_id%)
  LOCAL xtra% = Object_getExtra%(obj_id%)  

  LOCAL fire_countdown% = (xtra% AND HERO_FLG_FIRE_COUNTDOWN_MSK%)>>HERO_FLG_FIRE_COUNTDOWN_SHFT%

  IF fire_countdown% > 0 THEN
    EXIT SUB
  ENDIF
  
  LOCAL bullet% = ss_Bullet_create%(obj_position%(obj_id%))
  
  Object_setVelocity(bullet%, Vector_create%(MM.INFO(FONTWIDTH),0))
  
  Sound_play "../sounds/fire.wav", 0
END SUB
