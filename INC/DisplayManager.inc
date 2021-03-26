'Requires Utility.inc, Vector.inc, WorldManager.inc

DIM DM_is_started%=0

'Screen width in pixels
FUNCTION DM_getHorizontal%()
  DM_getHorizontal% = MM.HRES
END FUNCTION

'Screen height in pixels
FUNCTION DM_getVertical%()
  DM_getVertical% = MM.VRES
END FUNCTION

'Draw character at world location with color
SUB DM_drawCh(world_pos%, c$, colr%)
  LOCAL view_pos% = world_pos% - box_corner%(wm_view%)
  'TEXT Vector_getXint%(view_pos%), Vector_getYint%(view_pos%), c$, LT,,, colr%
  TEXT view_pos%>>>48, (view_pos%<<32)>>>48, c$, LT,,, colr%
END SUB

'Draw string centered at world location with color
SUB DM_drawString(world_pos%, s$, colr%)

  LOCAL view_pos% = world_pos% - box_corner%(wm_view%)
  
  'LM_writeLog("drawstr pos: "+hex$(view_pos%)+ ","+s$+", color:"+HEX$(colr%))
  TEXT view_pos%>>>48, (view_pos%<<32)>>>48, s$, CM,,, colr%, 0
END SUB

SUB DM_startUp()
  OPTION CONSOLE SERIAL
  MODE 7, 8
  PAGE WRITE 1
  CLS
  DM_is_started%=1
END SUB

SUB DM_shutDown()
  PAGE WRITE 0
  DM_is_started%=0  
END SUB

'Block in this SUB until Vsync
SUB DM_Vsync
  DO WHILE GETSCANLINE <= MM.VRES: LOOP
END SUB

DIM dm_shake_severity%=0 'Screen shake severity in pixels
DIM dm_shake_duration%=0 'Screen shake duration in frames

SUB DM_swapBuffers()
  IF dm_shake_duration%>0 THEN
    INC dm_shake_duration%, -1
    PAGE SCROLL 1, dm_shake_severity%*(RND-0.5), dm_shake_severity%*(RND-0.5), 0
  ENDIF
  
  PAGE COPY 1 to 0 ', B 'Not syncing to Vsync at this point. We'll do that separately.
  'Using this instead of CLS to avoid continuously clearing the serial console.
  BOX 0,0, MM.HRES, MM.VRES, 0,,0
END SUB



