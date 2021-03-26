OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE BOTH

#INCLUDE "../INC/LogManager.INC"

MODE 2, 8

LM_startUp

LM_writeLog "Creating clock"

DIM sleep_until! = TIMER+20
DO WHILE TIMER < sleep_until!
LOOP

DIM clock_res!
DIM res%, bits%

FOR res%=1 TO 10
  FOR bits%=8 TO 16 STEP 4
    IF res%=9 AND bits%=12 THEN
      CONTINUE FOR
    ENDIF
    MODE res%, bits%
    TIMER=0
    PAGE COPY 1 TO 0, I
    'CLS
    clock_res! = TIMER
    LM_writeLog "MODE "+STR$(res%)+","+STR$(bits%)+": Clock delta (us) after PAGE COPY 1 TO 0:"+STR$(clock_res!)
    TIMER=0
    PAGE COPY 0 TO 1, I
    'CLS
    clock_res! = TIMER
    LM_writeLog "MODE "+STR$(res%)+","+STR$(bits%)+": Clock delta (us) after PAGE COPY 0 TO 1:"+STR$(clock_res!)
  NEXT bits%
NEXT res%

TIMER=0
sleep_until! = TIMER+20
DO WHILE TIMER < sleep_until!
LOOP
clock_res! = TIMER
LM_writeLog "Clock after sleep(20):"+STR$(clock_res!)

TIMER=0
sleep_until! = TIMER+20
DO WHILE TIMER < sleep_until!
LOOP
clock_res! = TIMER
LM_writeLog "Clock split after sleep(20):"+STR$(clock_res!)

LM_shutDown

END

