'Set of recognized joystick actions
CONST EVT_JOY_ACT_UNDEFD% = 0
CONST EVT_JOY_ACT_CLICKED% = 1  'Button went from not pressed to pressed.
CONST EVT_JOY_ACT_PRESSED% = 2  'Button is currently pressed.
CONST EVT_JOY_ACT_MOVED% = 3

'Set of recognize buttons
CONST EVT_JOY_BUTTON_UNDEFD% = 0
CONST EVT_JOY_BUTTON_Z% = 1
CONST EVT_JOY_BUTTON_C% = 2

CONST EVT_JOY_ACT_MSK% = (3<<10) 'Event data starts at bit 10
CONST EVT_JOY_ACT_SHFT% = 10
CONST EVT_JOY_BUTTON_MSK% =(3<<12)
CONST EVT_JOY_BUTTON_SHFT% =12

CONST EVT_JOY_POSX_MSK%=(&H1FFFFFF<<39)
CONST EVT_JOY_POSX_SHFT%=39
CONST EVT_JOY_POSY_MSK%=(&H1FFFFFF<<14)
CONST EVT_JOY_POSY_SHFT%=14

SUB EVT_JOY_init(ev%)
  ev% = EVT_JOY% 'Event type
END SUB

'Set joystick event action
SUB EVT_JOY_setAction(ev%, action%)
  ev% = (ev% AND (INV EVT_JOY_ACT_MSK%)) OR (action%<<EVT_JOY_ACT_SHFT%)
END SUB

'Get joystick event action
FUNCTION EVT_JOY_getAction%(ev%)
  EVT_JOY_getAction% = (ev% AND EVT_JOY_ACT_MSK%) >> EVT_JOY_ACT_SHFT%
END FUNCTION

'Set joystick event button
SUB EVT_JOY_setButton(ev%, button%)
  ev% = (ev% AND (INV EVT_JOY_BUTTON_MSK%)) OR (button%<<EVT_JOY_BUTTON_SHFT%)
END SUB

'Get joystick event button
FUNCTION EVT_JOY_getButton%(ev%)
  EVT_JOY_getButton% = (ev% AND EVT_JOY_BUTTON_MSK%) >> EVT_JOY_BUTTON_SHFT%
END FUNCTION

'Set joystick event position. Position is a vector.
SUB EVT_JOY_setPosition(ev%, position%)
'We have 50 bits at our disposal, so 25 bits per component.
'Since the components are expected to be in the range (-1,1), we'll take the 9.16 portion
'of each component
  ev% = (ev% AND (INV (EVT_JOY_POSX_MSK% OR EVT_JOY_POSY_MSK%)))
  ev% = ev% OR ((position% << 7) AND EVT_JOY_POSX_MSK%) OR ((position% << EVT_JOY_POSY_SHFT%) AND EVT_JOY_POSY_MSK%)
END SUB

'Get joystick event position. Returned position is a vector.
FUNCTION EVT_JOY_getPosition%(ev%)
  'This is the reverse of the set operation above, with sign extension.
  EVT_JOY_getPosition% = ((ev% AND EVT_JOY_POSX_MSK%)>>>7) OR (((ev%<<25)>>>EVT_JOY_POSX_SHFT%) AND &HFFFFFFFF)
END FUNCTION

