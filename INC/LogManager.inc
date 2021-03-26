OPTION CONSOLE SERIAL

DIM LM_is_started% = 0

'Start up the Log Manager.
SUB LM_StartUp
  LM_is_started% = 1
END SUB

'Shut down the LogManager.
SUB LM_shutDown
  LM_is_started% = 0
END SUB

'Write to log.
SUB LM_writeLog(msg$)
  IF LM_is_started% THEN
    PRINT msg$
  ENDIF
END SUB

