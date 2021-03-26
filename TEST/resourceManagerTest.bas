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

RM_loadSprite("../sprites/orig.txt", "Saucer")
RM_loadSprite("../sprites/ship-spr.txt", "Ship")

IF Sprite_find%("Wrong") <> -1 THEN
  Error "Sprite_find error."
ENDIF

DIM sprid% = Sprite_find%("Ship")
IF sprid% = -1 THEN
  Error "Sprite_find error."
ENDIF

DIM frid%
frid% = Sprite_getFrame%(sprid%, 1)
'IF fobj$="" THEN
'  Error "Sprite_getFrame error"
'ENDIF

sprid% = Sprite_find%("Saucer")
IF sprid% = -1 THEN
  Error "Sprite_find error."
ENDIF

IF Sprite_getWidth%(sprid%) <> 6 THEN
  Error "Sprite_getWidth error."
ENDIF

IF Sprite_getHeight%(sprid%) <> 2 THEN
  Error "Sprite_getHeight error."
ENDIF

IF Sprite_getColor%(sprid%) <> RGB(GREEN) THEN
  Error "Sprite_getColor error."
ENDIF

IF Sprite_getLabel$(sprid%) <> "Saucer" THEN
  Error "Sprite_getColor error."
ENDIF

IF Sprite_getSlowdown%(sprid%) <> 4 THEN
  Error "Sprite_getSlowdown error."
ENDIF

IF Sprite_getFrameCount%(sprid%) <> 5 THEN
  ERROR "Sprite_getFrameCount error."
ENDIF

frid% = Sprite_getFrame%(sprid%, 0)
'IF fobj$="" THEN
'  Error "Sprite_getFrame error"
'ENDIF

IF Frame_getHeight%(frid%) <> 2 THEN
  ERROR "Frame_getHeight error"
ENDIF

IF Frame_getWidth%(frid%) <> 6 THEN
  ERROR "Frame_getHeight error"
ENDIF

DIM frame_longString%(20)
LONGSTRING CLEAR frame_longString%()
Frame_getLongString(frid%, frame_longString%())

IF LGETSTR$(frame_longString%(), 1, LLEN(frame_longString%())) <> " ____ /____\" THEN
  ERROR "Frame_getString error"
ENDIF

frid% = Sprite_getFrame%(sprid%, 4)
'IF fobj$="" THEN
'  Error "Sprite_getFrame error"
'ENDIF

IF Frame_getHeight%(frid%) <> 2 THEN
  ERROR "Frame_getHeight error"
ENDIF

IF Frame_getWidth%(frid%) <> 6 THEN
  ERROR "Frame_getHeight error"
ENDIF

LONGSTRING CLEAR frame_longString%()
Frame_getLongString(frid%, frame_longString%())

IF LGETSTR$(frame_longString%(), 1, LLEN(frame_longString%())) <> " ____ /o___\" THEN
  ERROR "Frame_getString error"
ENDIF

RM_unloadSprite("Saucer")
'RM_unloadSprite("Saucer")

'ON ERROR IGNORE
'RM_loadSprite("sprites/beginDelMissing.txt", "Saucer")
'RM_loadSprite("sprites/endDelMissing.txt", "Saucer")
'RM_loadSprite("sprites/colorMissing.txt", "Saucer")
'RM_loadSprite("sprites/missingEndFrame.txt", "Saucer")
'RM_loadSprite("sprites/bodyMissing.txt", "Saucer")

'RM_loadSprite("sprites/footerMissing.txt", "Saucer")
'RM_loadSprite("sprites/heightMissing.txt", "Saucer")
'RM_loadSprite("sprites/widthMissing.txt", "Saucer")
'RM_loadSprite("sprites/slowdownMissing.txt", "Saucer")

LM_writeLog("Test Complete.")
GM_shutDown()

END



