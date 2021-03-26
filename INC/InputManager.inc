'Requires Manager.inc, EventKeyboard.inc, EventJoystick.inc

CONST LCTRL_MASK% = 2
CONST RCTRL_MASK% = 32
CONST LALT_MASK% = 1
CONST RALT_MASK% = 16
CONST LSHFT_MASK% = 8
CONST RSHFT_MASK% = 128

CONST NUNCHUK_CHANNEL% = 3

DIM IM_is_started% = 0

DIM key_was_down%(6)
DIM key_is_down%(6)
DIM key_was_down_idx%
DIM key_is_down_idx%

DIM nunchuk_open%=0
DIM nunchuk_jxl%=0
DIM nunchuk_jxc%=0
DIM nunchuk_jxr%=0
DIM nunchuk_jyt%=0
DIM nunchuk_jyc%=0
DIM nunchuk_jyb%=0
DIM nunchuk_z_button_was_pressed%=0
DIM nunchuk_c_button_was_pressed%=0

'Get ready to capture input
SUB IM_startUp
  key_was_down_idx%=0
  key_is_down_idx%=0

  ON ERROR SKIP 1
  WII NUNCHUK OPEN
  
  IF MM.ERRNO=0 THEN 
    nunchuk_open%=1
  ELSE
    ON ERROR CLEAR
  ENDIF
  
  IF nunchuk_open% THEN
    LM_writeLog("IM: Nunchuk detected.")
    
    nunchuk_jxl% = NUNCHUK(JXL, 3)
    nunchuk_jxc% = NUNCHUK(JXC, 3)
    nunchuk_jxr% = NUNCHUK(JXR, 3)

    nunchuk_jyt% = NUNCHUK(JYT, 3)
    nunchuk_jyc% = NUNCHUK(JYC, 3)
    nunchuk_jyb% = NUNCHUK(JYB, 3)
    
    LM_writeLog("jxl="+STR$(nunchuk_jxl%)+" jxc="+STR$(nunchuk_jxc%)+" jxr=" +STR$(nunchuk_jxr%))
    LM_writeLog("jyt="+STR$(nunchuk_jyt%)+" jyc="+STR$(nunchuk_jyc%)+" jyb=" +STR$(nunchuk_jyb%))
  ENDIF
  
  IM_is_started%=1
END SUB

SUB IM_shutDown
  IF nunchuk_open% THEN
    WII NUNCHUK CLOSE
    nunchuk_open%=0
  ENDIF
  
  IM_is_started%=0
END SUB

'Get input and pass event to all Objects.
SUB IM_getInput
  keyboardInput
  
  IF nunchuk_open% THEN
    nunchukInput
  ENDIF
END SUB

SUB nunchukInput
  LOCAL ev%, dummy%
  
  EVT_JOY_init ev%
  
  'Check is nunchuk moved
  LOCAL jx% = NUNCHUK(JX, NUNCHUK_CHANNEL%) - nunchuk_jxc%
  LOCAL jy% = NUNCHUK(JY, NUNCHUK_CHANNEL%) - nunchuk_jyc%
  LOCAL jx_norm!, jy_norm!
  
  IF jx% < 0 THEN
    jx_norm! = jx%/(nunchuk_jxc%-nunchuk_jxl%)
  ELSE
    jx_norm! = jx%/(nunchuk_jxr%-nunchuk_jxc%)
  ENDIF

  IF jy% < 0 THEN
    jy_norm! = jy%/(nunchuk_jyc%-nunchuk_jyb%)
  ELSE
    jy_norm! = jy%/(nunchuk_jyt%-nunchuk_jyc%)
  ENDIF  
  
  'LM_writeLog("jx_norm="+STR$(jx_norm!)+" jy_norm="+STR$(jy_norm!))
  
  IF (jx_norm!>0.1) OR (jx_norm!<-0.1) OR (jy_norm!>0.1) OR (jy_norm!<-0.1) THEN
    EVT_JOY_setAction(ev%, EVT_JOY_ACT_MOVED%)
    EVT_JOY_setPosition(ev%, Vector_create%(jx_norm!, jy_norm!))
    dummy% = onEvent%(ev%)
  ENDIF
  
  'Check z button
  IF NUNCHUK(Z, NUNCHUK_CHANNEL%) THEN
    EVT_JOY_setAction(ev%, EVT_JOY_ACT_PRESSED%)
    EVT_JOY_setButton(ev%, EVT_JOY_BUTTON_Z%)
    dummy% = onEvent%(ev%)
    IF NOT nunchuk_z_button_was_pressed% THEN
      EVT_JOY_setAction(ev%, EVT_JOY_ACT_CLICKED%)    
      dummy% = onEvent%(ev%)
      nunchuk_z_button_was_pressed% = 1
    ENDIF
  ELSE
    nunchuk_z_button_was_pressed% = 0
  ENDIF

  'Check c button
  IF NUNCHUK(C, NUNCHUK_CHANNEL%) THEN
    EVT_JOY_setAction(ev%, EVT_JOY_ACT_PRESSED%)
    EVT_JOY_setButton(ev%, EVT_JOY_BUTTON_C%)
    dummy% = onEvent%(ev%)
    IF NOT nunchuk_c_button_was_pressed% THEN
      EVT_JOY_setAction(ev%, EVT_JOY_ACT_CLICKED%)    
      dummy% = onEvent%(ev%)
      nunchuk_c_button_was_pressed% = 1
    ENDIF
  ELSE
    nunchuk_c_button_was_pressed% = 0
  ENDIF  
END SUB

SUB keyboardInput
  LOCAL k% = 0
  LOCAL m% = 0
  LOCAL ii%=1, jj%, dummy%, ev%
  LOCAL numKeysPressed%=KEYDOWN(0)
  
  EVT_KBD_init ev%
  
  key_is_down_idx%=0

  DO WHILE ii% <= numKeysPressed%
    k% = KEYDOWN(ii%)
    IF k%<>0 THEN
      key_is_down%(key_is_down_idx%) = k%
      INC key_is_down_idx%
    ENDIF
    INC ii%
  LOOP

  m% = KEYDOWN(7)
    
  IF (m% AND LCTRL_MASK%) THEN
      key_is_down%(key_is_down_idx%) = LCTRL_KEY%
      INC key_is_down_idx% 
  ENDIF 

  IF (m% AND RCTRL_MASK%) THEN
      key_is_down%(key_is_down_idx%) = RCTRL_KEY%
      INC key_is_down_idx% 
  ENDIF 

  IF (m% AND LALT_MASK%) THEN
      key_is_down%(key_is_down_idx%) = LALT_KEY%
      INC key_is_down_idx% 
  ENDIF 

  IF (m% AND RALT_MASK%) THEN
      key_is_down%(key_is_down_idx%) = RALT_KEY%
      INC key_is_down_idx% 
  ENDIF 

  IF (m% AND LSHFT_MASK%) THEN
      key_is_down%(key_is_down_idx%) = LSHFT_KEY%
      INC key_is_down_idx% 
  ENDIF 

  IF (m% AND RSHFT_MASK%) THEN
      key_is_down%(key_is_down_idx%) = RSHFT_KEY%
      INC key_is_down_idx% 
  ENDIF 

  'PRINT "num keys pressed + modifiers = "+STR$(key_is_down_idx%)
  
  'Check for key release events
  ii%=0
  EVT_KBD_setAction ev%, EVT_KBD_ACT_KEY_RELEASED%
  
  DO WHILE ii%<key_was_down_idx%
    jj%=0
    DO WHILE jj%<key_is_down_idx%
      IF key_was_down%(ii%) = key_is_down%(jj%) THEN 'Still down
        GOTO key_not_just_released
      ENDIF
      INC jj%
    LOOP

    'No match so key was released
    EVT_KBD_setKey ev%, key_was_down%(ii%)
    dummy% = onEvent%(ev%)
    
key_not_just_released:
    INC ii%
  LOOP
  
  'Check for key press events
  ii%=0
  EVT_KBD_setAction ev%, EVT_KBD_ACT_KEY_PRESSED%
  
  DO WHILE ii%<key_is_down_idx%
    jj%=0
    DO WHILE jj%<key_was_down_idx%
      IF key_is_down%(ii%) = key_was_down%(jj%) THEN 'Still down
        GOTO key_not_just_pressed
      ENDIF
      INC jj%
    LOOP

    'No match so key was just pressed
    EVT_KBD_setKey ev%, key_is_down%(ii%)
    dummy% = onEvent%(ev%)

key_not_just_pressed:
    INC ii%
  LOOP

  'Generate key is down events
  ii%=0
  EVT_KBD_setAction ev%, EVT_KBD_ACT_KEY_DOWN%

  DO WHILE ii%<key_is_down_idx%
    EVT_KBD_setKey ev%, key_is_down%(ii%)
    dummy% = onEvent%(ev%)
    
    'Also copy key-is-down to key-was-down while we're at it.
    key_was_down%(ii%) = key_is_down%(ii%)
    INC ii%
  LOOP

  key_was_down_idx% = key_is_down_idx%
END SUB


