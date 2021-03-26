'Requires Vector.inc, Object.inc, Box.inc, WorldManager.inc

FUNCTION positionsIntersect%(v1%, v2%)
  LOCAL x1% = (v1%>>48) 'Vector_getXint%(v1%)
  LOCAL x2% = (v2%>>48) 'Vector_getXint%(v2%)
  LOCAL y1% = (v1%<<32)>>>48 'Vector_getYint%(v1%)
  LOCAL y2% = (v2%<<32)>>>48 'Vector_getYint%(v2%)
  LOCAL res% = 1
  
  IF x1%-x2% > 1 THEN
    res%=0
  ENDIF

  IF x2%-x1% > 1 THEN
    res%=0
  ENDIF

  IF y1%-y2% > 1 THEN
    res%=0
  ENDIF

  IF y2%-y1% > 1 THEN
    res%=0
  ENDIF
  
  positionsIntersect% = res%
END FUNCTION

'These are actually locals for the function below.
'made global for performance reasons
DIM u_ax1%, u_bx1%, u_ay1%, u_by1%

'params: box_a, box_b, result
CSUB boxIntersectBox INTEGER, INTEGER, INTEGER
  00000000
  F8D04B4C E92DC000 F8D34FF0 B08D8000 F8D8680B 9202E000 0ACCEB0E 09C3EB0E 
  B004F8DA A004F8D9 422BEA4F 903CF85E 442AEA4F E033F85E EA4F9200 EA4F72EB 
  920175EA 402EEA4F 2004F8D8 71EEEA4F 4629EA4F E008F8D8 EA4F9203 00DB0CCC 
  77E9EA4F AB00E9DD EB7B45A2 DA400205 44629A03 AB00E9DD 8900E9D2 0208EB1A 
  EB4B9208 92090209 8908E9DD EB7945A0 BFAC0205 24002401 EB774286 DA140201 
  E9DC44F4 18F33500 EB47930A 930B0305 230AE9DD 418B4282 2400DA14 9B022500 
  4500E9C3 E8BDB00D 44738FF0 5300E9D3 EB411942 92060303 E9DD9307 42B22306 
  DB1F41BB 9B0217E5 4500E9C3 E8BDB00D 9A038FF0 0A03EB02 8A00E9DA 0208EB14 
  EB459204 9205020A 8904E9DD AB00E9DD EB7945D0 DA0A020B BF0845AB D0A745A2 
  E7B92400 BF08428F D1C64286 2401E7B8 BF00E7B2 0800031C 
End CSUB

'Return true if boxes intersect
'FUNCTION boxIntersectBox%(box_a%, box_b%)
'  'LOCAL ax1% = Vector_getXint%(box_corner%(box_a%))
'  'LOCAL bx1% = Vector_getXint%(box_corner%(box_b%))
'  'LOCAL ay1% = Vector_getYint%(box_corner%(box_a%))
'  'LOCAL by1% = Vector_getYint%(box_corner%(box_b%))
'  u_ax1% = box_corner%(box_a%)>>>48: u_bx1% = box_corner%(box_b%)>>>48: u_ay1% = (box_corner%(box_a%)<<32)>>>48: u_by1% = (box_corner%(box_b%)<<32)>>>48  
'  'Test horizontal overlap
'  boxIntersectBox% = ((u_bx1%<=u_ax1%) AND (u_ax1%<=u_bx1% + box_horizontal%(box_b%))) OR ((u_ax1%<=u_bx1%) AND (u_bx1%<=u_ax1% + box_horizontal%(box_a%)))
'  'Test vertical overlap
'  boxIntersectBox% = boxIntersectBox% AND (((u_by1%<=u_ay1%) AND (u_ay1%<=u_by1% + box_vertical%(box_b%))) OR ((u_ay1%<=u_by1%) AND (u_by1%<=u_ay1% + box_vertical%(box_a%))))
'END FUNCTION

'Convert relative bounding box for object to absolute world box at given position
SUB getWorldBox(obj_id%, world_box_id%)
  'Box_copy(world_box_id%, Object_getBox%(obj_id%))
  Box_copy(world_box_id%, (obj_flags%(obj_id%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%)
  INC box_corner%(world_box_id%), obj_position%(obj_id%) 
END SUB

'Convert relative bounding box for Object to absolute world box at given position
'where is a vector.
SUB getWorldBoxAtPos(obj_id%, where%, world_box_id%)
  'Box_copy(world_box_id%, Object_getBox%(obj_id%))
  LOCAL from_box% = (obj_flags%(obj_id%) AND OBJ_FLG_BOX_MSK%) >> OBJ_FLG_BOX_SHFT%)
  box_corner%(world_box_id%) = box_corner%(from_box%) + where%
  box_horizontal%(world_box_id%) = box_horizontal%(from_box%)
  box_vertical%(world_box_id%) = box_vertical%(from_box%)
END SUB

'Return true if Box contains Position (p is a vector)
FUNCTION boxContainsPosition%(box_id%, p%)
  LOCAL corner% = box_corner%(box_id%)
  LOCAL bx1% = corner%>>>48 'Vector_getXint%(corner%)
  LOCAL by1% = (corner%<<32)>>>48 'Vector_getYint%(corner%)
  LOCAL bx2% = bx1% + box_horizontal%(box_id%)
  LOCAL by2% = by1% + box_vertical%(box_id%)
  LOCAL px% = p%>>>48 'Vector_getXint%(p%)
  LOCAL py% = (p%<<32)>>>48 'Vector_getYint%(p%)
  
  'Test horizontal overlap
  LOCAL x_overlap% = (px%>= bx1%) AND (px% <= bx2%)
  'Test vertical overlap
  LOCAL y_overlap% = (py%>= by1%) AND (py% <= by2%)

  boxContainsPosition% = x_overlap% AND y_overlap%
END FUNCTION

'Return true of Box a completely contains Box b
FUNCTION boxContainsBox%(box_a%, box_b%)
  LOCAL aCorner% = box_corner%(box_a%)
  LOCAL bCorner% = box_corner%(box_b%)
  
  LOCAL ax1% = aCorner%>>>48 'Vector_getXint%(aCorner%)
  LOCAL bx1% = bCorner%>>>48 'Vector_getXint%(bCorner%)
  LOCAL ay1% = (aCorner%<<32)>>>48 'Vector_getYint%(aCorner%)
  LOCAL by1% = (bCorner%<<32)>>>48 'Vector_getYint%(bCorner%)
  LOCAL ax2% = ax1% + box_horizontal%(box_a%)
  LOCAL bx2% = bx1% + box_horizontal%(box_b%)
  LOCAL ay2% = ay1% + box_vertical%(box_a%)
  LOCAL by2% = by1% + box_vertical%(box_b%)
  
  'Test horizontal overlap
  LOCAL x_overlap% = (ax1%<= bx1%) AND (ax2% >= bx2%)
  'Test vertical overlap
  LOCAL y_overlap% = (ay1%<= by1%) AND (ay2% >= by2%)
  
  boxContainsBox% = x_overlap% AND y_overlap%
END FUNCTION

'Convert world position to view position
FUNCTION worldToView%(world_pos%)
  worldToView% = world_pos% - box_corner%(wm_view%)
END FUNCTION

'Convert view position to world
FUNCTION viewToWorld%(view_pos%)
  viewToWorld% = view_pos% + box_corner%(wm_view%)
END FUNCTION

'/*Copy a block of bytes from one block with offset to another with offset.*/
'void blockCopy(long long *to, long long *to_offset, long long *from, long long *from_offset, long long *numBytes) {
'  char *toptr = ((char*)to) + *to_offset;
'  char *fromptr = ((char*)from) + *from_offset;
'  int ii;
'
'  for (ii=0; ii<*numBytes; ii++)
'    *(toptr + ii) = *(fromptr + ii);
'}
CSUB blockCopy INTEGER, INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  9E04B4F0 6809681F 4500E9D6 F1752C01 DB120300 1E4B3F01 25002400 4403443A 
  1F01F812 F8033401 F1451F01 E9D60500 42840100 0101EB75 BCF0DBF2 BF004770 
End CSUB

'/*Extract a string of given length from a block of bytes, with offset.*/
'void stringExtract(char *s, long long *from, long long *from_offset, long long *numBytesp) {
'  char *fromptr = ((char*)from) + *from_offset;
'  int ii=0, numBytes = (int)(*numBytesp);
'
'  *s++ = (char)numBytes;
'  
'  while (ii++<numBytes)
'    *s++ = *fromptr++;
'}
CSUB stringExtract STRING, INTEGER, INTEGER, INTEGER
  00000000
  6814B410 4421681A 70022A00 440ADD06 3B01F811 F8004291 D1F93F01 4B04F85D 
  BF004770 
End CSUB

