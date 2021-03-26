OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE SERIAL

#INCLUDE "../INC/Frame.inc"
#INCLUDE "../INC/Sprite.inc"
#INCLUDE "../INC/EventTypes.inc"
#INCLUDE "../INC/Event.inc"
#INCLUDE "../INC/EventStep.inc"
#INCLUDE "../INC/Object_types.inc"
#INCLUDE "../INC/Box.inc"
#INCLUDE "../INC/Animation.inc"
#INCLUDE "../INC/ViewEventTags.inc"
#INCLUDE "../INC/EventView.inc"
#INCLUDE "../INC/ViewObject.inc"
#INCLUDE "../INC/TextEntry.inc"
#INCLUDE "../INC/Object.inc"
#INCLUDE "../INC/ObjectList.inc"
#INCLUDE "../INC/ObjectListIterator.inc"
#INCLUDE "../INC/Utility.inc"
#INCLUDE "../INC/EventCollision.inc"
#INCLUDE "../INC/Manager.inc"
#INCLUDE "../INC/LogManager.inc"
#INCLUDE "../INC/DisplayManager.inc"
#INCLUDE "../INC/EventOut.inc"
#INCLUDE "../INC/SceneGraph.inc"
#INCLUDE "../INC/WorldManager.inc"
#INCLUDE "../INC/EventKeyboard.inc"
#INCLUDE "../INC/EventJoystick.inc"
#INCLUDE "../INC/InputManager.inc"
#INCLUDE "../INC/ResourceManager.inc"
#INCLUDE "../INC/Sound.inc"
#INCLUDE "../INC/GameManager.inc"
#INCLUDE "../INC/Vector.inc"

GM_startUp

DIM spr% = Sprite_create%()

Sprite_setWidth(spr%, 4)
Sprite_setHeight(spr%, 2)
Sprite_setColor(spr%, RGB(yellow))
Sprite_setLabel(spr%, "Test")
Sprite_setSlowdown(spr%, 2)

DIM frame%
DIM longstr%(20)
LONGSTRING CLEAR longstr%()
LONGSTRING APPEND longstr%(), "12344321"

frame% = Frame_create%(4, 2, longstr%())

Sprite_addFrame(spr%, frame%)

LONGSTRING CLEAR longstr%()
LONGSTRING APPEND longstr%(), "56788765"

frame% = Frame_create%(4, 2, longstr%())
Sprite_addFrame(spr%, frame%)

IF Sprite_getWidth%(spr%) <> 4 THEN
  Error "Sprite width incorrect"
ENDIF

IF Sprite_getHeight%(spr%) <> 2 THEN
  Error "Sprite height incorrect"
ENDIF

IF Sprite_getColor%(spr%) <> RGB(yellow) THEN
  Error "Sprite color incorrect"
ENDIF

IF Sprite_getLabel$(spr%) <> "Test" THEN
  Error "Sprite label incorrect"
ENDIF

IF Sprite_getSlowdown%(spr%) <> 2 THEN
  Error "Sprite slowdown incorrect"
ENDIF

IF Sprite_getFrameCount%(spr%) <> 2 THEN
  Error "Sprite framecount incorrect"
ENDIF

DIM frame2% = Sprite_getFrame%(spr%, 1)
IF frame2% <> frame% THEN
  Error "Sprite getFrame error."
ENDIF

LONGSTRING CLEAR longstr%()
LONGSTRING APPEND longstr%(), "abcddcba"
frame% = Frame_create%(4, 2, longstr%())
Sprite_addFrame(spr%, frame%)

DIM ii%=0
DIM v% = Vector_create%(DM_getHorizontal%()/2, DM_getVertical%()/2)

DO
  Sprite_draw(spr%, ii%, v%)
  INC ii%
  
  IF ii% >= Sprite_getFrameCount%() THEN
    ii%=0
  ENDIF
  
  DM_swapBuffers
  PAUSE 500
LOOP

END


