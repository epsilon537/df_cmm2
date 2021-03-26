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

DIM shipAnim% = Anim_create%()
DIM saucerAnim% = Anim_create%()

GM_startUp

RM_loadSprite("../sprites/orig.txt", "Saucer")

RM_loadSprite("../sprites/ship-spr.txt", "Ship")

DIM saucerSprid% = Sprite_find%("Saucer")
DIM shipSprid% = Sprite_find%("Ship")

IF Sprite_getSlowdown%(shipSprid%) <> 3 THEN
  Error "Sprite_getSlowdown error"
ENDIF

Anim_setSprite(saucerAnim%, Sprite_find%("Saucer"))
Anim_setSprite(shipAnim%, Sprite_find%("Ship"))

IF Anim_getSprite%(saucerAnim%) <> Sprite_find%("Saucer") THEN
  Error "Anim_getSprite error"
ENDIF

IF Anim_getSprite%(shipAnim%) <> Sprite_find%("Ship") THEN
  Error "Anim_getSprite error"
ENDIF

IF Anim_getIndex%(saucerAnim%) <> 0 THEN
  Error "Anim_getIndex error"
ENDIF

Anim_setIndex(shipAnim%, 1)
Anim_setIndex(saucerAnim%, 2)

IF Anim_getIndex%(shipAnim%) <> 1 THEN
  Error "Anim_getIndex error"
ENDIF

IF Anim_getIndex%(saucerAnim%) <> 2 THEN
  Error "Anim_getIndex error"
ENDIF

Anim_setSlowdownCount(saucerAnim%, -1)
IF Anim_getSlowdownCount%(saucerAnim%) <> -1 THEN
  Error "Anim_getSlowdownCount error. slowdowncount="+HEX$(Anim_getSlowdownCount%(saucerAnim%))
ENDIF

DIM v% = Vector_create%(100, 100)

Anim_draw(shipAnim%, v%)
IF Anim_getSlowdownCount%(shipAnim%) <> 1 THEN
  Error "Anim_getSlowdownCount error"
ENDIF
IF Anim_getIndex%(shipAnim%) <> 1 THEN
  Error "Anim_getIndex error"
ENDIF

Anim_draw(shipAnim%, v%)
IF Anim_getSlowdownCount%(shipAnim%) <> 2 THEN
  Error "Anim_getSlowdownCount error"
ENDIF
IF Anim_getIndex%(shipAnim%) <> 1 THEN
  Error "Anim_getIndex error"
ENDIF

Anim_draw(shipAnim%, v%)
IF Anim_getSlowdownCount%(shipAnim%) <> 0 THEN
  Error "Anim_getSlowdownCount error"
ENDIF
IF Anim_getIndex%(shipAnim%) <> 0 THEN
  Error "Anim_getIndex error"
ENDIF

Anim_draw(saucerAnim%, v%)

IF Anim_getSlowdownCount%(saucerAnim%) <> -1 THEN
  Error "Anim_getSlowdownCount error. slowdowncount="+HEX$(Anim_getSlowdownCount%(saucerAnim%))
ENDIF

IF Anim_getIndex%(saucerAnim%) <> 2 THEN
  Error "Anim_getIndex error"
ENDIF

Anim_destroy(shipAnim%)
Anim_destroy(saucerAnim%)

RM_unloadSprite("Saucer")
RM_unloadSprite("Ship")

LM_writeLog("Test Complete.")

GM_shutdown

END

