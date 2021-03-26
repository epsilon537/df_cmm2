OPTION EXPLICIT
OPTION DEFAULT NONE
OPTION BASE 0
OPTION CONSOLE BOTH

#INCLUDE "../INC/Vector.INC"
#INCLUDE "../INC/Box.INC"

DIM box1% = Box_create%(0, 0, 0)
DIM c1% = Vector_create%(3, 2)

box_corner%(box1%) = c1%
box_horizontal%(box1%) = 100
box_vertical%(box1%) = 200

IF box_corner%(box1%) <> c1% THEN
  ERROR "Box_getCorner error."
ENDIF

IF box_horizontal%(box1%) <> 100 THEN
  ERROR "Box_getHorizontal error"
ENDIF

IF box_vertical%(box1%) <> 200 THEN
  ERROR "Box_getVertical error"
ENDIF

DIM c2% = Vector_create%(10, 20)
DIM box2% = Box_create%(c2%, 1000, 2000)

IF num_boxes_allocated%<>2 THEN
  ERROR "box allocation error"
ENDIF

IF box_corner%(box2%) <> c2% THEN
  ERROR "Box_getCorner error."
ENDIF

IF box_horizontal%(box2%) <> 1000 THEN
  ERROR "Box_getHorizontal error"
ENDIF

IF box_vertical%(box2%) <> 2000 THEN
  ERROR "Box_getVertical error"
ENDIF

IF box_corner%(box1%) <> c1% THEN
  ERROR "Box_getCorner error."
ENDIF

IF box_horizontal%(box1%) <> 100 THEN
  ERROR "Box_getHorizontal error"
ENDIF

IF box_vertical%(box1%) <> 200 THEN
  ERROR "Box_getVertical error"
ENDIF

Box_destroy(box1%)
Box_destroy(box2%)

IF num_boxes_allocated%<>0 THEN
  ERROR "box allocation error"
ENDIF

PRINT "Test Complete."

END

