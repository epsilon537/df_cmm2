'Requires LogManager.inc, Frame.inc

CONST MAX_ROWS_IN_SPR_FILE_SECTION% = 80
CONST MAX_NUM_COLS_IN_SPRITE_FILE% = 80

CONST HEADER_TOKEN$ = "HEADER"
CONST BODY_TOKEN$ = "BODY"
CONST FOOTER_TOKEN$ = "FOOTER"
CONST FRAMES_TOKEN$ = "frames"
CONST HEIGHT_TOKEN$ = "height"
CONST WIDTH_TOKEN$ = "width"
CONST COLOR_TOKEN$ = "color"
CONST SLOWDOWN_TOKEN$ = "slowdown"
CONST END_FRAME_TOKEN$ = "end"
CONST VERSION_TOKEN$ = "version"

CONST COLOR_TOKEN_BLACK$ = "black"
CONST COLOR_TOKEN_RED$ = "red"
CONST COLOR_TOKEN_GREEN$ = "green"
CONST COLOR_TOKEN_YELLOW$ = "yellow"
CONST COLOR_TOKEN_BLUE$ = "blue"
CONST COLOR_TOKEN_MAGENTA$ = "magenta"
CONST COLOR_TOKEN_CYAN$ = "cyan"
CONST COLOR_TOKEN_WHITE$ = "white"

DIM RM_is_started%=0

'Trim any characters in c$ from the start of s$. Taken from CMM2 User Guide.
FUNCTION LTrim$(s$, c$)
  LTrim$ = s$
  DO WHILE INSTR(c$, LEFT$(LTrim$, 1))
    LTrim$ = MID$(LTrim$, 2)
  LOOP
END FUNCTION

'Read in next section of data from file as array of strings.
'Returns numberOfStrings read if OK, -1 otherwise.
'An empty array will return -1. An empty string will be added at the
'end of non_empty array so that even a single entry array will at least have DIM(1)
FUNCTION readData%(delimiter$, strings$())
  LOCAL begin$ = "<"+delimiter$+">" 'Section beginning
  LOCAL endDel$ = "</"+delimiter$+">" 'Section end
  LOCAL s$
  
  'Check for beginning
  LINE INPUT #1, s$
  IF s$ <> begin$ THEN
    LM_writeLog("Begin delimiter missing: "+begin$)
    readData%=-1
    EXIT FUNCTION
  ENDIF
  
  LOCAL ii%=0
  
  'Read in data until ending (or not found)
  LINE INPUT #1, s$
  
  DO WHILE s$<>endDel$ AND s$<>""
    strings$(ii%) = s$
    INC ii%
    LINE INPUT #1, s$    
  LOOP
  
  'If ending not found, then error.
  IF s$="" THEN
    LM_writeLog("End delimiter missing: "+endDel$)
    readData%=-1
    EXIT FUNCTION
  ENDIF

  IF ii%=0 THEN
    LM_writeLog("Empty section: "+begin$)
    readData%=-1
    EXIT FUNCTION
  ENDIF

  strings$(ii%) = ""
  readData%=ii%    
END FUNCTION

'Match token in vector of lines (e.g. "frames 5").
'Return corresponding value (e.g. 5), or -1 if not found
FUNCTION matchLineInt%(strings$(), numStrings%, token$)
  LOCAL number%=-1
  LOCAL ii%=0
  
  'Loop through all lines
  DO WHILE ii%<numStrings%
    IF INSTR(strings$(ii%), token$)=1 THEN
      number% = VAL(MID$(strings$(ii%), 1+LEN(token$)))
      'Consume item by setting it to empty string.
      strings$(ii%) = ""
      matchLineInt% = number%
      EXIT FUNCTION
    ENDIF
    
    INC ii%
  LOOP
  
  matchLineInt% = number%
END FUNCTION

'Match token in vector of lines (e.g. "color green").
'Return corresponding value (e.g. "green"), or "" if not found
FUNCTION matchLineStr$(strings$(), numStrings%, token$)
  LOCAL res$
  LOCAL ii%=0
  
  'Loop through all lines
  DO WHILE ii%<numStrings%
    IF INSTR(strings$(ii%), token$)=1 THEN
      res$ = MID$(strings$(ii%), 1+LEN(token$))
      'Consume item by setting it to empty string.
      strings$(ii%) = ""
      matchLineStr$ = LTrim$(res$, " ")
      EXIT FUNCTION
    ENDIF
    
    INC ii%
  LOOP
  
  matchLineStr$ = ""
END FUNCTION

'Match frame until "end", clearing all from vector.
'Return frame id, -1 on error.
FUNCTION matchFrame%(strings$(), numStrings%, width%, height%)
  LOCAL ii%, offset%=-1
  LOCAL lin$
  
  STATIC frame_longstr%(FRAME_OBJ_LEN_MAX_BYTES%\8)
  LONGSTRING CLEAR frame_longstr%()

  IF height% >= numStrings% THEN
      LM.writeLog("Not enough data for given height.")
      matchFrame% = -1
      EXIT FUNCTION
  ENDIF

  'Find first non-consumed line
  DO WHILE ii%<numStrings%
    IF strings$(ii%) <> "" THEN
      offset% = ii%
      EXIT DO
    ENDIF
    
    INC ii%
  LOOP
  
  IF offset%=-1 THEN
     LM.writeLog("All data consumed. No frame left to match")
     matchFrame% = -1
     EXIT FUNCTION
  ENDIF
  
  FOR ii%=offset% TO offset%+height%-1
    lin$ = strings$(ii%)
    
    IF LEN(lin$)<>0 AND LEN(lin$)<> WIDTH% THEN
      LM_writeLog("Incorrect frame width: "+lin$)
      matchFrame% = -1
      EXIT FUNCTION
    ENDIF
    
    strings$(ii%) = "" 'Consume
    
    IF LLEN(frame_longstr%()) + LEN(lin$) >= FRAME_OBJ_LEN_MAX_BYTES% THEN
      LM.writeLog("Frame is too long.")
      matchFrame% = -1
      EXIT FUNCTION
    ENDIF

    LONGSTRING APPEND frame_longstr%(), lin$      
  NEXT ii%
  
  IF strings$(offset%+height%) <> END_FRAME_TOKEN$ THEN
    LM_writeLog("Could not find end frame token.")
    matchFrame% = -1
    EXIT FUNCTION
  ENDIF
  
  strings$(offset%+height%) = "" 'Consume
  
  'Create and return the frame
  matchFrame% = Frame_create%(width%, height%, frame_longstr%())
END FUNCTION

SUB RM_startUp()
  RM_is_started%=1
END SUB

SUB RM_shutDown()
  LOCAL sprid%
  
  FOR sprid%=0 TO MAX_NUM_SPRITES%-1
    IF Sprite_isAllocated%(sprid%) THEN
      Sprite_destroy(sprid%)
    ENDIF
  NEXT sprid%
  
  RM_is_started%=0
END SUB

'Load sprite from file
'Assign indicated label to sprite.
SUB RM_loadSprite(filename$, label$)
  IF Sprite_find%(label$) <> -1 THEN
    ERROR "Sprite label already in use: "+label$
  ENDIF
  
  LM_writeLog("Loading sprite file: "+filename$)
  
  OPEN filename$ FOR INPUT AS #1
  
  STATIC strings$(MAX_ROWS_IN_SPR_FILE_SECTION%)
  
  'Header has sprite format data
  LOCAL numStrings% = readData%(HEADER_TOKEN$, strings$())
  IF numStrings% <=0 THEN
    Error "Failed to read section of file: "+HEADER_TOKEN$
  ENDIF
  
  LOCAL frames% = matchLineInt%(strings$(), numStrings%, FRAMES_TOKEN$)
  IF frames% = -1 THEN
    Error "Could not find field in file: "+FRAMES_TOKEN$
  ENDIF 

  LOCAL height% = matchLineInt%(strings$(), numStrings%, HEIGHT_TOKEN$)
  IF height% = -1 THEN
    Error "Could not find field in file: "+HEIGHT_TOKEN$
  ENDIF 

  LOCAL width% = matchLineInt%(strings$(), numStrings%, WIDTH_TOKEN$)
  IF width% = -1 THEN
    Error "Could not find field in file: "+WIDTH_TOKEN$
  ENDIF 
  
  LOCAL colorStr$ = matchLineStr$(strings$(), numStrings%, COLOR_TOKEN$)
  IF colorStr$ = "" THEN
    Error "Could not find field in file: "+COLOR_TOKEN$
  ENDIF 
  
  LOCAL colr%
  
  SELECT CASE colorStr$
    CASE COLOR_TOKEN_BLACK$
      colr% = RGB(BLACK)    
    CASE COLOR_TOKEN_RED$
      colr% = RGB(RED)    
    CASE COLOR_TOKEN_GREEN$
      colr% = RGB(GREEN)    
    CASE COLOR_TOKEN_YELLOW$
      colr% = RGB(YELLOW)    
    CASE COLOR_TOKEN_BLUE$
      colr% = RGB(BLUE)    
    CASE COLOR_TOKEN_MAGENTA$
      colr% = RGB(MAGENTA)    
    CASE COLOR_TOKEN_CYAN$
      colr% = RGB(CYAN)
    CASE COLOR_TOKEN_WHITE$
      colr% = RGB(WHITE)
    CASE ELSE
      colr% = RGB(NOTBLACK)
  END SELECT
  
  LOCAL slowdown% = matchLineInt%(strings$(), numStrings%, SLOWDOWN_TOKEN$)
  IF slowdown% = -1 THEN
    Error "Could not find field in file: "+SLOWDOWN_TOKEN$
  ENDIF 

  'Body has frame data
  numStrings% = readData%(BODY_TOKEN$, strings$())
  IF numStrings% <=0 THEN
    Error "Failed to read section of file: "+BODY_TOKEN$
  ENDIF
  
  LOCAL sprid% = Sprite_create%()
  LOCAL ii%
  LOCAL frid%
  
  FOR ii%=0 TO frames%-1
    frid% = matchFrame%(strings$(), numStrings%, width%, height%)
    IF frid% = -1 THEN
      ERROR "Failed to parse frame data for sprite "+label$
    ENDIF
    
    Sprite_addFrame(sprid%, frid%)
  NEXT ii%
  
  Sprite_setColor(sprid%, colr%)
  Sprite_setHeight(sprid%, height%)
  Sprite_setWidth(sprid%, width%)
  Sprite_setSlowdown(sprid%, slowdown%)
  Sprite_setLabel(sprid%, label$)

  CLOSE #1
END SUB

'Unload sprite with indicated label.
SUB RM_unloadSprite(label$)
  LOCAL sprid% = Sprite_find%(label$)
  
  IF sprid% = -1 THEN
    Error "Sprite label "+label$+" not found."
  ENDIF
  
  Sprite_destroy sprid%
END SUB

