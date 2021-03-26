
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
#INCLUDE "../INC/xprof.inc"

#INCLUDE "ss_Saucer.inc"
#INCLUDE "ss_Explosion.inc"
#INCLUDE "ss_Bullet.inc"
#INCLUDE "ss_EventNuke.inc"
#INCLUDE "ss_Hero.inc"
#INCLUDE "ss_Points.inc"
#INCLUDE "ss_Star.inc"
#INCLUDE "ss_GameStart.inc"
#INCLUDE "ss_GameOver.inc"

CONST MAX_NUM_CMDLINE_ARGS% = 20
DIM cmdLineArgs$(MAX_NUM_CMDLINE_ARGS%)
DIM nArgs%, argIdx%
DIM arg$

parseCmdLine(MM.CMDLINE$, cmdLineArgs$(), nArgs%)

FOR argIdx%=0 TO nArgs%-1
  SELECT CASE UCASE$(cmdLineArgs$(argIdx%))
    CASE "L"
      GM_displayLoopTime%=1
    CASE "P"
      GM_enableProfiling%=1
  END SELECT
NEXT argIdx%

GM_startUp 'Start game engine

''Set world boundary 2 screens high
'DIM corner% = Vector_create%(0,0)
'DIM world_boundary% = Box_create%(corner%, DM_getHorizontal%(), DM_getVertical%()*2)
'Box_copy(wm_boundary%, world_boundary%)

loadResources 'Load game resources
populateWorld 'Populate game world with some objects

GM_run

GM_shutDown 'Shutdown game engine

END

SUB loadResources
  RM_loadSprite("../sprites/saucer-spr.txt","saucer")
  RM_loadSprite("../sprites/ship-spr.txt","ship")
  RM_loadSprite("../sprites/bullet-spr.txt","bullet")
  RM_loadSprite("../sprites/explosion-spr.txt","explosion")
  RM_loadSprite("../sprites/gamestart-spr.txt","gamestart")
  RM_loadSprite("../sprites/gameover-spr.txt","gameover")
END SUB

SUB populateWorld
  LOCAL ii%, dummy%
  
  FOR ii%=0 TO 10
    dummy% = ss_Star_create%()
  NEXT ii%

  dummy% = ss_GameStart_create%()
END SUB

SUB parseCmdLine(cmdLine$, cmdLineArgs$(), nArgs%)
  LOCAL curPos%=1, startPos%
  LOCAL inWhiteSpace%=1
  LOCAL curArg%=0
  
  DO WHILE (curPos%<=LEN(cmdLine$)) AND (curArg%<MAX_NUM_CMDLINE_ARGS%)
    IF inWhiteSpace% THEN
      IF MID$(cmdLine$, curPos%, 1) <> " " THEN
        startPos% = curPos%
        inWhiteSpace% = 0
      ENDIF
    ELSE
      IF MID$(cmdLine$, curPos%, 1) = " " THEN
        cmdLineArgs$(curArg%) = MID$(cmdLine$, startPos%, curPos%-startPos%)
        INC curArg%
        inWhiteSpace% = 1
      ENDIF
    ENDIF
    INC curPos%
  LOOP
  
  IF (inWhiteSpace%=0) AND (curArg% < MAX_NUM_CMDLINE_ARGS%) THEN
    cmdLineArgs$(curArg%) = MID$(cmdLine$, startPos%)
    INC curArg%
  ENDIF
  
  nArgs% = curArg%
END SUB

