OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE BOTH

#INCLUDE "../INC/Vector.INC"

DIM v%, v2%, v3%

Vector_setXY(v%, 2.5, 3.5)
Vector_setXY(v2%, 1.1, 2.2)

PRINT "(2.5, 3.5), x="+STR$(Vector_getX!(v%))+" y="+STR$(Vector_getY!(v%))

v3% = v%+v2%
PRINT "(3.6, 5.7), x="+STR$(Vector_getX!(v3%))+" y="+STR$(Vector_getY!(v3%))

v3% = v%-v2%
PRINT "(1.4, 1.3), x="+STR$(Vector_getX!(v3%))+" y="+STR$(Vector_getY!(v3%))

v3% = v2%-v%
PRINT "(-1.4, -1.3), x="+STR$(Vector_getX!(v3%))+" y="+STR$(Vector_getY!(v3%))
PRINT "Length of this vector:"+STR$(Vector_getMagnitude!(v3%))

Vector_normalize(v3%)
PRINT "This vector normalized:x="+STR$(Vector_getX!(v3%))+" y="+STR$(Vector_getY!(v3%))
PRINT "Length of this vector:"+STR$(Vector_getMagnitude!(v3%))

v3% = Vector_scale%(v3%, 0.5)
PRINT "This vector scaled by 0.5: x="+STR$(Vector_getX!(v3%))+" y="+STR$(Vector_getY!(v3%))
PRINT "Length of this vector:"+STR$(Vector_getMagnitude!(v3%))

END

