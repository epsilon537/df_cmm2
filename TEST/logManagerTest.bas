OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE BOTH

#INCLUDE "../INC/LogManager.INC"

LM_writeLog "Before startup"

LM_startUp                       
LM_writeLog "LM_isStarted="+STR$(LM_is_Started%)

LM_writeLog "After startup"
LM_writeLog "with params: "+STR$(42)+" "+STR$(3.14)

LM_shutDown

LM_writeLog "After shutdown"
LM_writeLog "LM_isStarted="+STR$(LM_is_Started%)

END

