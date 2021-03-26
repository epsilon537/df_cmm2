'Requires LogManager.inc, EventStep.inc, WorldManager.inc, Manager.inc, InputManager.inc, DisplayManager.inc, ResourceManager.inc, Clock.inc, Box.inc

CONST VSYNC_MARGIN_MS! = 1.0 'How long in advance we should start waiting for Vsync.
'Default frame time (game loop time) in ms (40ms=25fps)
CONST FRAME_TIME_DEFAULT_MS! = (40-VSYNC_MARGIN_MS!)

DIM GM_game_over%=0 'Game Over Status
DIM GM_frame_time_ms!=FRAME_TIME_DEFAULT_MS! 'Frame time is target time for game loop in ms.
DIM GM_frame_count%=0 'Current frame counter
DIM GM_is_started% = 0

DIM GM_displayLoopTime%=0
DIM GM_enableProfiling%=0

'Start all GameManager services.
SUB GM_startUp
  Mgr_startUp
  LM_startUp
  WM_startUp
  DM_startUp
  IM_startUp
  RM_startUp
  
  'Set world and view size to DM view dimensions
  box_horizontal%(wm_boundary%) = DM_getHorizontal%()
  box_vertical%(wm_boundary%) = DM_getVertical%()
  Box_copy(wm_view%, wm_boundary%)
  
  GM_is_started% = 1
END SUB

'Shut down GameManager services.
SUB GM_shutDown
  GM_setGameOver(1)
  Sound_stop
  RM_shutDown
  IM_shutDown
  DM_shutDown
  WM_shutDown
  LM_shutDown
  Mgr_shutDown
  GM_is_started% = 0
END SUB

'Run game loop
SUB GM_run
  LOCAL dummy%, ev_step%
  LOCAL intended_sleep_time_ms!, actual_sleep_time_ms!, adjust_time_ms!=0
  LOCAL loop_time_ms!, sleep_until_ms!

  IF GM_enableProfiling% THEN
    xprof_start(1, 50)
  ENDIF
      
  DO WHILE NOT GM_game_over%
    'dummy% = Clock_delta_us%() 'Reset clock.    
    IF NOT GM_enableProfiling% THEN
      DM_Vsync 'Sync to screen refresh.
    ENDIF
    'loop_time_us% = Clock_split_us%()
    'LM_writeLog("Vsync wait time us: "+STR$(loop_time_us%))
    
    loop_time_ms! = TIMER
    INC GM_frame_count%

    DM_swapBuffers 'We do this during the Vsync retrace so there are no visible artifacts.

    'Send a step event to all objects.    
    EventStep_init(ev_step%, GM_frame_count%)

    dummy% = onEvent%(ev_step%)

    IM_getInput
    WM_update
    WM_draw
            
    loop_time_ms! = TIMER - loop_time_ms!    
    IF GM_displayLoopTime% THEN
      TEXT 0,0, STR$(loop_time_ms!)+" ms"
    ENDIF

    IF NOT GM_enableProfiling% THEN    
      'Sleep slightly less than GM_frame_time so we're on time for the Vsync
      intended_sleep_time_ms! = MAX(0, GM_frame_time_ms! - loop_time_ms! - adjust_time_ms!)
      loop_time_ms! = TIMER: sleep_until_ms! = loop_time_ms! + intended_sleep_time_ms!
      DO WHILE TIMER<sleep_until_ms!: LOOP
      'LM_writeLog("Sleep Time us = "+STR$(actual_sleep_time_us%))
      'LM_writeLog("Intended Sleep Time us = "+STR$(intended_sleep_time_us%))
      adjust_time_ms! = MAX(0, TIMER - loop_time_ms! - intended_sleep_time_ms!)
    ENDIF
  LOOP
END SUB

'Set Game Over status to indicated value. True stops game loop
SUB GM_setGameOver(new_game_over%)
  GM_game_over% = new_game_over%
END SUB

