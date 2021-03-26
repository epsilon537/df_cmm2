CONST SOUND_STOPPED%=0
CONST SOUND_PLAYING%=1

DIM sound_curFile$ = ""
DIM sound_playInLoop% = 0
DIM sound_stop_ack% = 0
DIM sound_stop_req% = 0

'Sound play completed/stopped interrupt routine.
SUB Sound_play_int  
  IF sound_stop_req%=1 THEN
    sound_stop_req%=0
    PLAY WAV sound_curFile$ 'Remove interrupt routine
    PLAY STOP 'Immediately stop, before previous play command actual start does anything
    sound_stop_ack%=1
    EXIT SUB
  ENDIF
  
  IF sound_playInloop% THEN
    PLAY WAV sound_curFile$, Sound_play_int
  ELSE
    'Reset sound_curFile to indicate that nothing's currently playing anymore
    sound_curFile$=""
  ENDIF
END SUB

'This is the API to play a sound.
SUB Sound_play(filename$, loopSound%)
  Sound_stop
  
  sound_curFile$ = filename$
  sound_playInLoop% = loopSound%
  
  PLAY WAV sound_curFile$, Sound_play_int
END SUB

SUB Sound_stop
  'Is anything playing?
  IF sound_curFile$ <> "" THEN
    sound_stop_req%=1 'Request a sound stop from ISR
    PLAY STOP 'Trigger ISR
    'Wait for ack
    DO WHILE sound_stop_ack%=0: LOOP
    'Reset flags
    sound_stop_ack%=0 
    
    sound_curFile$=""
    sound_playInLoop% = 0  
  ENDIF
END SUB

