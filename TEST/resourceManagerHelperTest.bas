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

DIM strings$(100)
DIM res%, numStrings%
DIM s$
DIM frid%

LM_startUp()

OPEN "../sprites/beginDelMissing.txt" FOR INPUT as #1

numStrings% = readData%(HEADER_TOKEN$, strings$())

IF numStrings% <> -1 THEN
  Error "beginDelMissing test failed."
ENDIF

CLOSE #1

OPEN "../sprites/endDelMissing.txt" FOR INPUT as #1

numStrings% = readData%(HEADER_TOKEN$, strings$())

IF numStrings% <> -1 THEN
  Error "endDelMissing test failed."
ENDIF

CLOSE #1

OPEN "../sprites/orig.txt" FOR INPUT as #1

numStrings% = readData%(HEADER_TOKEN$, strings$())

IF numStrings% <= 0 THEN
  Error "orig header test failed."
ENDIF

res% = matchLineInt%(strings$(), numStrings%, FRAMES_TOKEN$)
IF res% <> 5 THEN
  Error "orig frames test failed."
ENDIF

res% = matchLineInt%(strings$(), numStrings%, HEIGHT_TOKEN$)
IF res% <> 2 THEN
  Error "orig height test failed."
ENDIF

res% = matchLineInt%(strings$(), numStrings%, WIDTH_TOKEN$)
IF res% <> 6 THEN
  Error "orig width test failed."
ENDIF

res% = matchLineInt%(strings$(), numStrings%, SLOWDOWN_TOKEN$)
IF res% <> 4 THEN
  Error "orig slowdown test failed."
ENDIF

s$ = matchLineStr$(strings$(), numStrings%, COLOR_TOKEN$)
IF s$ <> "green" THEN
  Error "orig color test failed. Received: "+s$
ENDIF

numStrings% = readData%(BODY_TOKEN$, strings$())

IF numStrings% <= 0 THEN
  Error "orig body test failed."
ENDIF

frid% = matchFrame%(strings$(), numStrings%, 6, 2)
IF frid% = -1 THEN
  Error "orig matchFrame test failed."
ENDIF

numStrings% = readData%(FOOTER_TOKEN$, strings$())

IF numStrings% <= 0 THEN
  Error "orig footer test failed."
ENDIF

res% = matchLineInt%(strings$(), numStrings%, VERSION_TOKEN$)
IF res% <> 1 THEN
  Error "orig version test failed."
ENDIF

CLOSE #1

OPEN "../sprites/framesMissing.txt" FOR INPUT as #1

numStrings% = readData%(HEADER_TOKEN$, strings$())

IF numStrings% <= 0 THEN
  Error "framesMissing header test failed."
ENDIF

res% = matchLineInt%(strings$(), numStrings%, FRAMES_TOKEN$)
IF res% <> -1 THEN
  Error "orig framesMissing test failed."
ENDIF

CLOSE #1

OPEN "../sprites/colorMissing.txt" FOR INPUT as #1

numStrings% = readData%(HEADER_TOKEN$, strings$())

IF numStrings% <= 0 THEN
  Error "colorMissing header test failed."
ENDIF

s$ = matchLineStr$(strings$(), numStrings%, COLOR_TOKEN$)
IF s$ <> "" THEN
  Error "colorMissing test failed."
ENDIF

CLOSE #1

OPEN "../sprites/incorrectWidth.txt" FOR INPUT as #1

numStrings% = readData%(HEADER_TOKEN$, strings$())
numStrings% = readData%(BODY_TOKEN$, strings$())
IF numStrings% <= 0 THEN
  Error "incorrectWidth body test failed."
ENDIF

frid% = matchFrame%(strings$(), numStrings%, 6, 2)
IF frid% <> -1 THEN
  Error "incorrectWidth matchFrame test failed."
ENDIF

CLOSE #1

OPEN "../sprites/missingEndFrame.txt" FOR INPUT as #1

numStrings% = readData%(HEADER_TOKEN$, strings$())
numStrings% = readData%(BODY_TOKEN$, strings$())
IF numStrings% <= 0 THEN
  Error "missingEndFrame body test failed."
ENDIF

frid% = matchFrame%(strings$(), numStrings%, 6, 2)
IF frid% <> -1 THEN
  Error "missingEndFrame matchFrame test failed."
ENDIF

CLOSE #1

PRINT "Test Complete."
END


