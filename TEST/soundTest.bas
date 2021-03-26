OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE BOTH

#INCLUDE "../INC/Sound.inc"

PRINT "Play unlooped."
Sound_play("../sounds/start-music.wav",0)
PAUSE 30000
PRINT "Play looped."
Sound_play("../sounds/start-music.wav",1)
PAUSE 30000
PRINT "Pause"
PLAY PAUSE
PAUSE 3000
PRINT "Resume"
PLAY RESUME
PAUSE 3000
Sound_stop
PRINT "Done."
END
