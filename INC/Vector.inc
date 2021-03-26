'Initialize a Vector object, which fits into an integer type, to be treated as an opaque value type.
SUB Vector_setXY(v%, x!, y!)
  'Store vector as two Fix16.16 numbers, x in the upper 32 bits, y in the lower.
  v% = (CINT(x!*65536)<<32) OR (CINT(y!*65536) AND &HFFFFFFFF)
END SUB

FUNCTION Vector_create%(x!, y!)
  'Store vector as two Fix16.16 numbers, x in the upper 32 bits, y in the lower.
  Vector_create% = (CINT(x!*65536)<<32) OR (CINT(y!*65536) AND &HFFFFFFFF)
END FUNCTION

SUB Vector_setX(v%, x!)
  v% = (CINT(x!*65536)<<32) OR (v% AND &HFFFFFFFF)
END SUB

SUB Vector_setY(v%, y!)
  v% = (v% AND &HFFFFFFFF00000000) OR (CINT(y!*65536) AND &HFFFFFFFF)
END SUB

FUNCTION Vector_getX!(v%)
  Vector_getX! = (v%>>>32)/65536
END FUNCTION

FUNCTION Vector_getY!(v%)
  'Sign extend if necessary.
  Vector_getY! = ((v%<<32)>>>32)/65536
END FUNCTION

FUNCTION Vector_getXint%(v%)
  Vector_getXint% = v%>>>48
END FUNCTION

FUNCTION Vector_getYint%(v%)
  'Sign extend if necessary.
  Vector_getYint% = (v%<<32)>>>48
END FUNCTION

FUNCTION Vector_getMagnitude!(v%)
  LOCAL x! = (v%>>>32)/65536
  LOCAL y! = ((v%<<32)>>>32)/65536
  Vector_getMagnitude! = SQR(x!*x! + y!*y!)
END FUNCTION

SUB Vector_normalize(v%)
  LOCAL l! = Vector_getMagnitude!(v%)
  IF (l! > 0) THEN
    LOCAL x% = CINT(v%/l!) AND &HFFFFFFFF00000000
    LOCAL y% = CINT(v% AND &HFFFFFFFF)
    IF y% AND &H80000000 THEN
      y% = (y% OR &HFFFFFFFF00000000)/l!
    ELSE
      y% = y%/l!    
    ENDIF
    v% = x% OR (y% AND &HFFFFFFFF)
  ENDIF
END SUB

FUNCTION Vector_scale%(v%, s!)
    LOCAL x% = CINT(v%*s!) AND &HFFFFFFFF00000000
    LOCAL y% = CINT(v% AND &HFFFFFFFF)
    'Sign extend if necessary
    IF y% AND &H80000000 THEN
      y% = (y% OR &HFFFFFFFF00000000)*s!
    ENDIF
    Vector_scale% = x% OR (y% AND &HFFFFFFFF)
END FUNCTION

