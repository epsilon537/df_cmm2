
'Enumeration of Event Types
CONST EVT_UNDEFD% = 0
CONST EVT_TEST% = 1
CONST EVT_STEP% = 2
CONST EVT_KBD% = 3
CONST EVT_COL% = 4
CONST EVT_OUT% = 5
CONST EVT_VIEW% = 6
CONST EVT_JOY% = 7

CONST EVT_SS_NUKE% = 8

CONST MAX_NUM_EVT_TYPES% = 32

DIM EVT_NAMES$(MAX_NUM_EVT_TYPES%) LENGTH 16

EVT_NAMES$(EVT_UNDEFD%) = "Undefined Evt."
EVT_NAMES$(EVT_TEST%) = "Test Evt."
EVT_NAMES$(EVT_STEP%) = "Step Evt."
EVT_NAMES$(EVT_KBD%) = "Keyboard Evt."
EVT_NAMES$(EVT_COL%) = "Collision Evt."
EVT_NAMES$(EVT_OUT%) = "Out Evt."
EVT_NAMES$(EVT_JOY%) = "Joystick Evt."
EVT_NAMES$(EVT_SS_NUKE%) = "SS Nuke Evt."


