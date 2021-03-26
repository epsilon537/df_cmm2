CONST MAX_NUM_BOXES%=1000

DIM box_corner%(MAX_NUM_BOXES%-1)
DIM box_horizontal%(MAX_NUM_BOXES%-1) 'A value of -1 means not allocated
SUB box_horizontalInit
  LOCAL ii%
  
  FOR ii%=0 TO MAX_NUM_BOXES%-1
    box_horizontal%(ii%) = -1
  NEXT ii%
END SUB
box_horizontalInit

DIM box_vertical%(MAX_NUM_BOXES%-1)

'Params: box_corner, box_horizontal, box_vertical
CSUB boxIntersectBoxInit INTEGER, INTEGER, INTEGER
  00000000
  B4104B05 6020681C F85D6818 60414B04 609A681B BF004770 0800031C 
End CSUB
boxIntersectBoxInit box_corner%(0), box_horizontal%(0), box_vertical%(0)

'Index into table above used to find the next available (unallocated) box 
DIM box_allocate_index%=0

'Used to track resource leaks
DIM num_boxes_allocated%=0

'Allocate a box from the box heap and initialize it. Returns a box id.
'corner is a vector, horizontal and vertical are integers.
FUNCTION Box_create%(corner%, horizontal%, vertical%)
  LOCAL ii% = box_allocate_index%
  
  DO
    IF box_horizontal%(ii%) = -1 THEN 'Free entry found
      'Allocate and set attributes
      box_horizontal%(ii%) = horizontal%
      box_vertical%(ii%) = vertical%
      box_corner%(ii%) = corner%
      'Update index to next hopefully free entry
      box_allocate_index% = ii% + 1
      IF box_allocate_index% >= MAX_NUM_BOXES% THEN
        box_allocate_index%=0
      ENDIF
    
      INC num_boxes_allocated%  
      Box_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= MAX_NUM_BOXES% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = box_allocate_index%
  
  ERROR "Box Heap exhausted!"
END FUNCTION

'Destroy given box, releasing all resources.
SUB Box_destroy(box_id%)
  IF box_horizontal%(box_id%) = -1 THEN
    ERROR "Attempt to free a Box that was not allocated."
  ENDIF
  
  box_horizontal%(box_id%) = -1
  
  INC num_boxes_allocated%, -1  
END SUB

'Copy one box's attributes to another
SUB Box_copy(to_box%, from_box%)
  box_corner%(to_box%) = box_corner%(from_box%)
  box_horizontal%(to_box%) = box_horizontal%(from_box%)
  box_vertical%(to_box%) = box_vertical%(from_box%)
END SUB

