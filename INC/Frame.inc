'Requires Vector.inc, DisplayManager.inc, Utility.inc

CONST MAX_NUM_FRAMES% = 200
CONST FRAME_OBJ_LEN% = 1024\8
CONST MAX_FRAME_WIDTH% = 200
CONST FRAME_WIDTH_OFFSET% = FRAME_OBJ_LEN%-2
CONST FRAME_HEIGHT_OFFSET% = FRAME_OBJ_LEN%-1
'-2 because last two words are frame width and height.
CONST FRAME_OBJ_LEN_MAX_BYTES% = (FRAME_OBJ_LEN%-2)*8

'The list of frames. 1st index is frame data, 2nd index is frame id.
DIM frames%(FRAME_OBJ_LEN%-1, MAX_NUM_FRAMES%-1)

'Index into table above used to find the next available (unallocated) frame object
DIM frames_allocate_index%=0

'Used to track resource leaks
DIM num_frames_allocated%=0

'A string buffer used when rendering frame
DIM frame_string_buf$

'Create a frame object of indicated width, height and long string
FUNCTION Frame_create%(width%, height%, longstr%())
  LOCAL ii% = frames_allocate_index%

  IF (width%=0) OR (width% > MAX_FRAME_WIDTH%) OR (height%=0) THEN
    ERROR "Can't create frame with width or height of 0"
  ENDIF

  IF LLEN(longstr%()) > FRAME_OBJ_LEN_MAX_BYTES% THEN
    ERROR "Frame create input string too long: "+STR$(LLEN(longstr%()))
  ENDIF
  
  DO
    IF frames%(FRAME_WIDTH_OFFSET%, ii%) = 0 THEN 'Free entry found
      'Allocate by storing width and height at the end of the array
      frames%(FRAME_WIDTH_OFFSET%, ii%) = width%
      frames%(FRAME_HEIGHT_OFFSET%, ii%) = height%

      blockCopy frames%(0, ii%), 0, longstr%(), 8, LLEN(longstr%())
   
      'Update index to next hopefully free entry
      frames_allocate_index% = ii% + 1
      IF frames_allocate_index% >= MAX_NUM_FRAMES% THEN
        frames_allocate_index%=0
      ENDIF
      
      INC num_frames_allocated%
      Frame_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= MAX_NUM_FRAMES% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = frames_allocate_index%
  
  ERROR "Frames Heap exhausted!"
END FUNCTION

'Destroy given frame object, releasing all resources used by this object.
SUB Frame_destroy(frid%)
  IF frames%(FRAME_WIDTH_OFFSET%, frid%) = 0 THEN
    ERROR "Attempt to free an Frame Object that was not allocated."
  ENDIF
  
  frames%(FRAME_WIDTH_OFFSET%, frid%) = 0
  
  INC num_frames_allocated%, -1  
END SUB

'Set width of frame
'Pass in frame id and width value
SUB Frame_setWidth(frid%, width%)
  IF (width%=0) OR (width% > MAX_FRAME_WIDTH%) THEN
    ERROR "Invalid frame width."
  ENDIF

  frames%(FRAME_WIDTH_OFFSET%, frid%) = width%
END SUB

'Get width of frame
'Pass in frame id
FUNCTION Frame_getWidth%(frid%)
  Frame_getWidth% = frames%(FRAME_WIDTH_OFFSET%, frid%)
END FUNCTION

'Set height of frame
'Pass in frame id and height value
SUB Frame_setHeight(frid%, height%)
  IF height%=0 THEN
    ERROR "Invalid frame Height."
  ENDIF

  frames%(FRAME_HEIGHT_OFFSET%, frid%) = height%
END SUB

'Get height of frame
'Pass in frame id
FUNCTION Frame_getHeight%(frid%)
  Frame_getHeight% = frames%(FRAME_HEIGHT_OFFSET%, frid%)
END FUNCTION

'Set frame characters
'Pass in frame id and longstring containing the frame characters
SUB Frame_setLongString(frid%, longstr%())
  IF LLEN(longstr%()) > FRAME_OBJ_LEN_MAX_BYTES% THEN
    ERROR "Frame create input string too long: "+STR$(LLEN(longstr%()))
  ENDIF

  blockCopy frames%(0, frid%), 0, longstr%(), 8, LLEN(longstr%())  
END SUB

'Get frame longstring
'Pass in frame id and longstring to copy to.
SUB Frame_getLongString(frid%, longstr%())
  'Note the -1 because that's how MMBasic works.
  LONGSTRING RESIZE longstr%(), frames%(FRAME_HEIGHT_OFFSET%, frid%)*frames%(FRAME_WIDTH_OFFSET%, frid%) -1
  blockCopy longstr%(), 8, frames%(0, frid%), 0, LLEN(longstr%())
END SUB

'Draw frame, centered at position (Vector), with given color
SUB Frame_draw(frid%, position%, colr%)
  LOCAL x_pos% = position%>>>48 'Vector_getXint%(position%)
  LOCAL y_pos% = (position%<<32)>>>48 'Vector_getYint%(position%)

  LOCAL w% = frames%(FRAME_WIDTH_OFFSET%, frid%)
  LOCAL h% = frames%(FRAME_HEIGHT_OFFSET%, frid%)
  LOCAL char_height% = MM.INFO(FONTHEIGHT)
  LOCAL y_offset% = y_pos% - h%*char_height%\2
  LOCAL x% = x_pos% - w%*MM.INFO(FONTWIDTH)\2

  'Draw row by row
  LOCAL row%    
  FOR row%=0 TO h%-1
    stringExtract frame_string_buf$, frames%(0, frid%), row%*w%, w%
    '1st arg = Vector_create%(x%, y_offset% + row%*char_height%)
    DM_drawCh((x%<<48) OR ((y_offset% + row%*char_height%)<<16), frame_string_buf$, colr%)
  NEXT row%  
END SUB  

