'Requires Sprite.inc, Box.inc, DisplayManager.inc

'Animation decoding masks and shifts
CONST ANIM_SPRID_MSK% = 1023
CONST ANIM_SPRID_SHFT% = 0
CONST ANIM_IDX_MSK% = (255<<10)
CONST ANIM_IDX_SHFT% = 10
CONST ANIM_BOX_MSK% = (1023<<18)
CONST ANIM_BOX_SHFT% = 18
CONST ANIM_SLOWDOWN_MSK% = (255<<56)
CONST ANIM_SLOWDOWN_SHFT% = 56

FUNCTION Anim_create%()
  LOCAL anim%=0
  LOCAL box_id% = Box_create%(0, 0, 0)

  anim% = (box_id%<<ANIM_BOX_SHFT%) OR ANIM_SPRID_MSK%
  Anim_create% = anim%
END FUNCTION

SUB Anim_destroy(anim%)
  LOCAL box_id% = (anim% AND ANIM_BOX_MSK%) >> ANIM_BOX_SHFT%
  Box_destroy(box_id%)
END SUB

'Set associated Sprite. Sprite index is set to 0.
SUB Anim_setSprite(anim%, sprid%)
  anim% = (anim% AND (INV ANIM_SPRID_MSK%) AND (INV ANIM_IDX_MSK%)) OR sprid%
  
  LOCAL box_id% = (anim% AND ANIM_BOX_MSK%) >> ANIM_BOX_SHFT%
  box_horizontal%(box_id%) = Sprite_getWidth%(sprid%)*MM.INFO(FONTWIDTH)
  box_vertical%(box_id%) = Sprite_getHeight%(sprid%)*MM.INFO(FONTHEIGHT)
END SUB

'Return associated sprite id.
FUNCTION Anim_getSprite%(anim%)
  Anim_getSprite% = anim% AND ANIM_SPRID_MSK%
END FUNCTION

'Set index of Sprite frame to be displayed
SUB Anim_setIndex(anim%, index%)
  anim% = (anim% AND (INV ANIM_IDX_MSK%)) OR (index% << ANIM_IDX_SHFT%)
END SUB

'Get index of current Sprite frame
FUNCTION Anim_getIndex%(anim%)
  Anim_getIndex% = (anim% AND ANIM_IDX_MSK%) >> ANIM_IDX_SHFT%
END FUNCTION

'Set animation slowdown count (-1=stop)
SUB Anim_setSlowdownCount(anim%, count%)
  count% = (count% << ANIM_SLOWDOWN_SHFT%) AND ANIM_SLOWDOWN_MSK%
  anim% = (anim% AND (INV ANIM_SLOWDOWN_MSK%)) OR count%
END SUB

'Get animation slowdown count (-1=stop)
FUNCTION Anim_getSlowdownCount%(anim%)
  Anim_getSlowdownCount% = anim% >>> ANIM_SLOWDOWN_SHFT%
END FUNCTION

DIM anim_index%, anim_count% 'Locals made global for performance

'Draw single frame centered at given position (Vector).
'Account for slowdown and advance sprite frame.
SUB Anim_draw(anim%, position%)
  LOCAL sprid% = anim% AND ANIM_SPRID_MSK%
  
  IF sprid% = ANIM_SPRID_MSK% THEN: ERROR "Animation object has no sprite: "+STR$(anim%): ENDIF
  
  anim_index% = (anim% AND ANIM_IDX_MSK%) >> ANIM_IDX_SHFT%

  Sprite_draw sprid%, anim_index%, position%
  
  anim_count% = (anim% AND ANIM_SLOWDOWN_MSK%) >>> ANIM_SLOWDOWN_SHFT%
  
  IF anim_count% = -1 THEN: EXIT SUB: ENDIF 
  INC anim_count%

  IF anim_count% >= Sprite_getSlowdown%(sprid%) THEN
    anim_count% = 0
    INC anim_index%
    'If at last frame, loop to beginning
    IF anim_index% >= Sprite_getFrameCount%(sprid%) THEN: anim_index% = 0: ENDIF
  ENDIF
  
  anim% = (anim% AND (INV (ANIM_SLOWDOWN_MSK% OR ANIM_IDX_MSK%))) OR (anim_index% << ANIM_IDX_SHFT%) OR (anim_count% << ANIM_SLOWDOWN_SHFT%)
END SUB

'Get animation bounding box
FUNCTION Anim_getBox%(anim%)
  Anim_getBox% = (anim% AND ANIM_BOX_MSK%) >> ANIM_BOX_SHFT%
END FUNCTION

