'Requires: ViewObject.inc, Object.inc, ObjectTypes.inc, EventTypes.inc, Event.inc

'Text Entry is a singleton ViewObject extension.

CONST TEXT_ENTRY_DEFAULT_BLINK_RATE% = 15

DIM textEntry_text$ 'Text entered
DIM textEntry_prefix$ 'Prefix going before text entry section
DIM textEntry_limit% = 0 'Character limit in text
DIM textEntry_numbersOnly% = 0 'True if only numbers
DIM textEntry_cursor% = 0 'Cursor location
DIM textEntry_cursorChar$ = "_" 'Cursor character
DIM textEntry_blinkRate% = TEXT_ENTRY_DEFAULT_BLINK_RATE% 'Cursor blink rate
DIM textEntry_callback$ 'Set to SUB to CALL back when enter is pressed.
DIM textEntry_obj_id% = -1

'Create a TextEntry Object. Call this before using any other TextEntry functions.
'Defaults: prefix="", limit=0, numbers_only=false, cursor_char=_, blink_rate=default
'Returns ViewObject id.
FUNCTION TextEntry_create%()
  IF textEntry_obj_id% <> -1 THEN
    ERROR "A TextEntry object already exists."
  ENDIF
  
  textEntry_obj_id% = ViewObject_create%()
  Object_setType(textEntry_obj_id%, OBJ_TYPE_TEXT_ENTRY%)
  
  textEntry_text$ = ""
  textEntry_prefix$ = ""
  textEntry_limit% = 0
  textEntry_numbersOnly% = 0
  textEntry_cursor% = 0
  textEntry_cursorChar$ = "_"
  textEntry_blinkRate% = TEXT_ENTRY_DEFAULT_BLINK_RATE%
  textEntry_callback$=""
  
  Object_registerInterest(textEntry_obj_id%, EVT_STEP%)
  Object_registerInterest(textEntry_obj_id%, EVT_KBD%)
  
  TextEntry_create% = textEntry_obj_id%
END FUNCTION

'Destroy TextEntry Object
SUB TextEntry_destroy(obj_id%)
  ViewObject_destroy(obj_id%)
  textEntry_obj_id% = -1
END SUB

'Set prefix
SUB TextEntry_setPrefix(obj_id%, prefix$)
  textEntry_prefix$ = prefix$
  PRINT "prefx = "+textEntry_prefix$+" LEN: "+STR$(LEN(textEntry_prefix$))
  PRINT "txt = "+textEntry_text$+" LEN: "+STR$(LEN(textEntry_text$))

  ViewObject_setViewString(obj_id%, textEntry_prefix$+textEntry_text$)
END SUB

'Set text entered
SUB TextEntry_setText(obj_id%, txt$)
  textEntry_text$ = LEFT$(txt$, textEntry_limit%)
  ViewObject_setViewString(obj_id%, textEntry_prefix$+textEntry_text$)
END SUB

FUNCTION TextEntry_eventHandler%(obj_id%, ev%)
  'If step event, blink cursor
  IF Event_getType%(ev%) = EVT_STEP% THEN
    'Blink on or off based on rate.
    STATIC blink%=-1*textEntry_blinkRate%

    IF blink% >= 0 THEN
      MID$(textEntry_text$, textEntry_cursor%+1, 1) = textEntry_cursorChar$
    ELSE
      MID$(textEntry_text$, textEntry_cursor%+1, 1) = " "
    ENDIF

    INC blink%
    
    IF blink% = textEntry_blinkRate% THEN
      blink% = -1*textEntry_blinkRate%
    ENDIF

    ViewObject_setViewString(obj_id%, textEntry_prefix$+textEntry_text$)
    TextEntry_eventHandler% = 1
    EXIT FUNCTION
  ENDIF
  
  'If keyboard event, handle
  IF Event_getType%(ev%) = EVT_KBD% THEN
    LOCAL action% = EVT_KBD_getAction%(ev%)

    IF action% = EVT_KBD_ACT_KEY_PRESSED% THEN
      LOCAL key% = EVT_KBD_getKey%(ev%)

      'If return key pressed, then callback  
      IF key% = ENTER_KEY% THEN
        IF textEntry_callback$ <> "" THEN
          LM_writeLog("TextEntry invoking callback.")
          CALL(textEntry_callback$)
        ENDIF
        
        TextEntry_eventHandler% = 1
        EXIT FUNCTION
      ENDIF
 
      IF key% = BCKSPC_KEY% THEN
        IF textEntry_cursor% > 0 THEN
          MID$(textEntry_text$, textEntry_cursor%+1, 1) = " "
          INC textEntry_cursor%, -1
          MID$(textEntry_text$, textEntry_cursor%+1, 1) = " "
          ViewObject_setViewString(obj_id%, textEntry_prefix$+textEntry_text$)              
        ENDIF
        
        TextEntry_eventHandler% = 1
        EXIT FUNCTION
      ENDIF
      
      'If no room, cannot add characters
      IF textEntry_cursor% >= textEntry_limit%-1 THEN
        TextEntry_eventHandler% = 1
        EXIT FUNCTION
      ENDIF
      
      'IF entry should be number, confirm
      IF textEntry_numbersOnly% AND NOT ((key%>= NUM0_KEY%) AND (key% <= NUM9_KEY%)) THEN
        TextEntry_eventHandler% = 1
        EXIT FUNCTION
      ENDIF

      'Replace with printable character
      IF (key% >= SPC_KEY%) AND (key% <= Z_KEY%) THEN
        MID$(textEntry_text$, textEntry_cursor%+1, 1) = CHR$(key%)
        ViewObject_setViewString(obj_id%, textEntry_prefix$+textEntry_text$)              
        INC textEntry_cursor%
      ENDIF
      
      TextEntry_eventHandler% = 1
      
      EXIT FUNCTION
    ENDIF      
  ENDIF

  TextEntry_eventHandler% = 0
  EXIT FUNCTION  
END FUNCTION

'Set limit of number of characters allowed.
SUB TextEntry_setLimit(obj_id%, limit%)
  textEntry_cursor% = 0
  textEntry_limit% = limit%
  textEntry_text$ = ""

  LOCAL ii%
  FOR ii%=0 TO limit%-1
    textEntry_text$ = textEntry_text$ + " "
  NEXT ii%

  ViewObject_setViewString(obj_id%, textEntry_prefix$+textEntry_text$)                
END SUB



