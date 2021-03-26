'Requires Frame.inc

CONST MAX_NUM_SPRITES% = 100 'Max. num. sprite objects that can exist in the game.
CONST MAX_NUM_SPRITE_FRAMES% = 16 'Max. number of frames that a sprite can hold.

'Sprite flags decoding masks and shifts
CONST SPR_FLG_ALLOCATED_MSK% = 1   'Sprite Object is allocated flag
CONST SPR_FLG_ALLOCATED_SHFT% = 0
CONST SPR_FLG_WIDTH_MSK% = (255<<1)   'Sprite width
CONST SPR_FLG_WIDTH_SHFT% = 1
CONST SPR_FLG_HEIGHT_MSK% = (255<<9)   'Sprite height
CONST SPR_FLG_HEIGHT_SHFT% = 9
CONST SPR_FLG_FRAMES_MSK% = (31<<17)   'Number of frames
CONST SPR_FLG_FRAMES_SHFT% = 17
CONST SPR_FLG_SLOWDOWN_MSK% = (15<<22)   'Sprite slowdown
CONST SPR_FLG_SLOWDOWN_SHFT% = 22
CONST SPR_FLG_COLOR_MSK% = (&HFFFFFFFF<<26)   'Sprite color
CONST SPR_FLG_COLOR_SHFT% = 26

'Various collision state values packed into an int. See mask and shift values above.
DIM spr_flags%(MAX_NUM_SPRITES%-1) 
DIM spr_label$(MAX_NUM_SPRITES%-1) LENGTH 16 'Text label to identify sprite.
DIM spr_frames%(MAX_NUM_SPRITE_FRAMES%-1, MAX_NUM_SPRITES%-1) 'Array of frames

'Index into tables above used to find the next available (unallocated) object 
DIM spr_allocate_index%=0

'Used to track resource leaks
DIM num_sprs_allocated%=0

'Allocate a sprite object from the heap and initialize it with given params.
FUNCTION Sprite_create%()
  LOCAL ii% = spr_allocate_index%
  
  DO
    IF spr_flags%(ii%) = 0 THEN 'Free entry found
      'Allocate
      spr_flags%(ii%) = 1
      'label
      spr_label$(ii%) = ""
      
      'Update index to next hopefully free entry
      spr_allocate_index% = ii% + 1
      IF spr_allocate_index% >= MAX_NUM_SPRITES% THEN
        spr_allocate_index%=0
      ENDIF
    
      INC num_sprs_allocated%  
      Sprite_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= MAX_NUM_SPRITES% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = spr_allocate_index%
  
  ERROR "Sprite Object Heap exhausted!"
END FUNCTION

'Destroy given sprite object, releasing all resources used by this object.
SUB Sprite_destroy(sprid%)
  IF spr_flags%(sprid%) = 0 THEN
    ERROR "Attempt to free an Sprite Object that was not allocated."
  ENDIF

  LOCAL ii%
  
  FOR ii%=0 TO Sprite_getFrameCount%(sprid%)-1
    'Release associated frame objects
    Frame_destroy(Sprite_getFrame%(sprid%, ii%))
  NEXT ii%
  
  spr_flags%(sprid%) = 0
  
  INC num_sprs_allocated%, -1  
END SUB

'Returns non-zero if sprite object is allocated.
FUNCTION Sprite_isAllocated%(sprid%)
  Sprite_isAllocated% = spr_flags%(sprid%)
END FUNCTION

'Set width of sprite.
SUB Sprite_setWidth(sprid%, width%)
  spr_flags%(sprid%) = (spr_flags%(sprid%) AND (INV SPR_FLG_WIDTH_MSK%)) OR (width%<<SPR_FLG_WIDTH_SHFT%)
END SUB

'Get width of sprite.
FUNCTION Sprite_getWidth%(sprid%)
  Sprite_getWidth% = (spr_flags%(sprid%) AND SPR_FLG_WIDTH_MSK%) >> SPR_FLG_WIDTH_SHFT%
END FUNCTION

'Set height of sprite.
SUB Sprite_setHeight(sprid%, height%)
  spr_flags%(sprid%) = (spr_flags%(sprid%) AND (INV SPR_FLG_HEIGHT_MSK%)) OR (height%<<SPR_FLG_HEIGHT_SHFT%)
END SUB

'Get height of sprite.
FUNCTION Sprite_getHeight%(sprid%)
  Sprite_getHeight% = (spr_flags%(sprid%) AND SPR_FLG_HEIGHT_MSK%) >> SPR_FLG_HEIGHT_SHFT%
END FUNCTION

'Set color of sprite.
SUB Sprite_setColor(sprid%, colr%)
  spr_flags%(sprid%) = (spr_flags%(sprid%) AND (INV SPR_FLG_COLOR_MSK%)) OR (colr%<<SPR_FLG_COLOR_SHFT%)
END SUB

'Get color of sprite.
FUNCTION Sprite_getColor%(sprid%)
  Sprite_getColor% = (spr_flags%(sprid%) AND SPR_FLG_COLOR_MSK%) >> SPR_FLG_COLOR_SHFT%
END FUNCTION

'Get number of frames in sprite.
FUNCTION Sprite_getFrameCount%(sprid%)
  Sprite_getFrameCount% = (spr_flags%(sprid%) AND SPR_FLG_FRAMES_MSK%) >> SPR_FLG_FRAMES_SHFT%
END FUNCTION

'Add frame to sprite.
SUB Sprite_addFrame(sprid%, frid%)
  LOCAL frameCount% = Sprite_getFrameCount%(sprid%)
  
  IF frameCount% >= MAX_NUM_SPRITE_FRAMES% THEN
    Error "Can't add frame. Sprite full."
  ENDIF
  
  spr_frames%(frameCount%, sprid%) = frid%
  
  INC frameCount%
  spr_flags%(sprid%) = (spr_flags%(sprid%) AND (INV SPR_FLG_FRAMES_MSK%)) OR (frameCount%<<SPR_FLG_FRAMES_SHFT%)
END SUB

'Get frame indicated by number
FUNCTION Sprite_getFrame%(sprid%, frameNumber%)
  Sprite_getFrame% = spr_frames%(frameNumber%, sprid%)
END FUNCTION

'Set label associated with sprite
SUB Sprite_setLabel(sprid%, label$)
  spr_label$(sprid%) = label$
END SUB

'Get label associated with sprite
FUNCTION Sprite_getLabel$(sprid%)
  Sprite_getLabel$ = spr_label$(sprid%) 
END FUNCTION

'Set slowdown of sprite.
SUB Sprite_setSlowdown(sprid%, slowdown%)
  spr_flags%(sprid%) = (spr_flags%(sprid%) AND (INV SPR_FLG_SLOWDOWN_MSK%)) OR (slowdown%<<SPR_FLG_SLOWDOWN_SHFT%)
END SUB

'Get slowdown of sprite.
FUNCTION Sprite_getSlowdown%(sprid%)
  Sprite_getSlowdown% = (spr_flags%(sprid%) AND SPR_FLG_SLOWDOWN_MSK%) >> SPR_FLG_SLOWDOWN_SHFT%
END FUNCTION

'Draw indicated frame centered at position (Vector)
SUB Sprite_draw(sprid%, frameNumber%, position%)  
  Frame_draw(spr_frames%(frameNumber%, sprid%), position%, Sprite_getColor%(sprid%))
END SUB

'Find sprite by label. Returns sprite id, -1 if not found.
FUNCTION Sprite_find%(label$)
  LOCAL ii%
  
  FOR ii%=0 TO MAX_NUM_SPRITES%-1
    IF spr_flags%(ii%) AND (spr_label$(ii%) = label$) THEN
      Sprite_find% = ii%
      EXIT FUNCTION
    ENDIF
  NEXT ii%
  
  Sprite_find% = -1
END FUNCTION
