CONST MAX_OBJ_LIST_SIZE% = 100 'Note: each list element can hold up to 4 object ids.
CONST MAX_NUM_OBJS_IN_LIST% = (MAX_OBJ_LIST_SIZE%*4)
CONST MAX_NUM_OBJ_LISTS% = 100

'The list of objectLists. 1st index is object id. 2nd index is list id.
DIM objList_list%(MAX_OBJ_LIST_SIZE%-1, MAX_NUM_OBJ_LISTS%-1)

'Number of elements in the object list. Set to -1 for not allocated entries
DIM objList_numElems%(MAX_NUM_OBJ_LISTS%-1)
SUB objList_numElemsInit
  LOCAL ii%
  
  FOR ii%=0 TO MAX_NUM_OBJ_LISTS%-1
    objList_numElems%(ii%) = -1
  NEXT ii%
END SUB
objList_numElemsInit

'Index into table above used to find the next available (unallocated) objectList
DIM objList_allocate_index%=0

'Used to track resource leaks
DIM num_objLists_allocated%=0

'void delBytesFromBlock(long long *block, long long *blockOffset, long long *blockSizeInBytes, long long *numBytesToDelp) {
'  char *tptr = ((char*)block) + *blockOffset;
'  long long numBytesToDel = *numBytesToDelp;
'  char *endPtr = ((char*)block) + *blockSizeInBytes - numBytesToDel;
'  
'  /*byte-by-byte*/
'  while (tptr < endPtr) {
'    *tptr = *(tptr+numBytesToDel);
'    ++tptr;
'  }
'}
CSUB delBytesFromBlock INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  6814B410 680B681A 44031AA4 42834420 3A01D207 F812441A F8031F01 42981B01 
  F85DD1F9 47704B04 
End CSUB

'/*Byte compare two memory blocks. Returns non-zero in res if equal.*/
'void blockCompareEQ(long long *lhs, long long *rhs, long long *numBytes, long long *res) {
'  int numWords = (*numBytes)/8;
'  int remainder = (*numBytes)&7;
'  char *lptr = (char*)(lhs+numWords);
'  char *rptr = (char*)(rhs+numWords);
'  int ii;
'
'  for (ii=0; ii<numWords; ii++) {
'    if (lhs[ii] != rhs[ii]) {
'      *res = 0;
'      return;
'    }
'  }
'  
'  for (ii=0; ii<remainder; ii++) {
'    if (lptr[ii] != rptr[ii]) {
'      *res = 0;
'      return;
'    }
'  }
'
'  *res = 1;
'  return;
'}
CSUB blockCompareEQ INTEGER, INTEGER, INTEGER, INTEGER
  00000000
  43F0E92D E000F8D2 46746855 F1752C00 DA030200 0407F11E 0500F145 EA4208E2 
  F1BC7C45 DD160F00 0508F1A0 0408F1A1 E0012200 D00E4594 E9F53201 E9F48902 
  45B96702 45B0BF08 2000D0F4 E9C32100 E8BD0100 F01E83F0 D0110E07 0ECCEB0E 
  01CCEB01 040EEB00 02CCEB00 42A2E001 F812D006 F8115B01 42850B01 E7E4D0F7 
  21002001 0100E9C3 83F0E8BD 
End CSUB

'Allocate an objectList from the objectList heap and initialize it. Returns an objList id.
FUNCTION ObjList_create%()
  LOCAL ii% = objList_allocate_index%

  DO
    IF objList_numElems%(ii%) = -1 THEN 'Free entry found
      'Allocate, num elems = 0
      objList_numElems%(ii%) = 0
      
      'Update index to next hopefully free entry
      objList_allocate_index% = ii% + 1
      IF objList_allocate_index% >= MAX_NUM_OBJ_LISTS% THEN
        objList_allocate_index%=0
      ENDIF
      
      INC num_objLists_allocated%
      ObjList_create% = ii%
      EXIT FUNCTION 
    ENDIF
    
    INC ii%
    IF ii% >= MAX_NUM_OBJ_LISTS% THEN
      ii%=0
    ENDIF
  LOOP UNTIL ii% = objList_allocate_index%
  
  ERROR "ObjectList Heap exhausted!"
END FUNCTION

'Destroy given objList, releasing all resources used by this objList.
SUB ObjList_destroy(objList_id%)
  IF objList_numElems%(objList_id%) = -1 THEN
    ERROR "Attempt to free an ObjectList that was not allocated."
  ENDIF
  
  INC num_objLists_allocated%, -1
  objList_numElems%(objList_id%) = -1
END SUB

'Returns non-zero if objectList is allocated.
FUNCTION ObjList_isAllocated%(objList_id%)
  ObjList_isAllocated% = objList_numElems%(objList_id%) <> -1
END FUNCTION

SUB ObjList_copy(from_objLid%, to_objLid%)
  LOCAL numBytes% = objList_numElems%(from_objLid%)*2
  LOCAL toOffset%=0, fromOffset%=0
  
  objList_numElems%(to_objLid%) = objList_numElems%(from_objLid%)
    
  blockCopy(objList_list%(0, to_objLid%), toOffset%, objList_list%(0, from_objLid%), fromOffset%, numBytes%)
END SUB

SUB ObjList_insert(objList_id%, obj_id%)
  LOCAL numElems% = objList_numElems%(objList_id%)
  IF numElems% >= MAX_NUM_OBJS_IN_LIST% THEN
    ERROR "ObjList id="+STR$(objList_id%)+" is full."
  ENDIF
  
  LOCAL real_index% = numElems%\4
  LOCAL shift% = 16*(numElems% AND 3)

  objList_list%(real_index%, objList_id%) = (objList_list%(real_index%, objList_id%) AND (INV (65535<<shift%))) OR (obj_id%<<shift%)
  INC objList_numElems%(objList_id%), 1
END SUB

'Returns objId at given index in objList (not range checked!)
FUNCTION ObjList_get%(objList_id%, index%)
  ObjList_get% = (objList_list%(index%>>2, objList_id%) >> (16*(index% AND 3))) AND 65535
END FUNCTION

'Returns list index of given object, or -1 if not found.
FUNCTION ObjList_find%(objList_id%, obj_id%)
  LOCAL num_elems% = objList_numElems%(objList_id%)
  LOCAL real_num_words% = (num_elems%+3)\4
  LOCAL ii%=0, index%, test_word%
  
  'Go through 4 find attempts, each one at a different word offset.
  DO WHILE ii% < real_num_words%
    test_word% = objList_list%(ii%, objList_id%)
    
    IF (test_word% AND 65535) = obj_id% THEN
      ObjList_find% = ii%*4
      EXIT FUNCTION
    ENDIF

    IF (test_word% AND (65535<<16)) = (obj_id%<<16) THEN
      index% = ii%*4 + 1
      ObjList_find% = CHOICE(index% < num_elems%, index%, -1)
      EXIT FUNCTION
    ENDIF

    IF (test_word% AND (65535<<32)) = (obj_id%<<32) THEN
      index% = ii%*4 + 2
      ObjList_find% = CHOICE(index% < num_elems%, index%, -1)
      EXIT FUNCTION
    ENDIF

    IF (test_word% AND (65535<<48)) = (obj_id%<<48) THEN
      index% = ii%*4 + 3
      ObjList_find% = CHOICE(index% < num_elems%, index%, -1)
      EXIT FUNCTION
    ENDIF
    
    INC ii%
  LOOP
  
  ObjList_find% = -1
END FUNCTION

'Remove object from list. Returns 0 if success, -1 if obj_id not found.
FUNCTION ObjList_remove%(objList_id%, obj_id%)
  LOCAL index% = ObjList_find%(objList_id%, obj_id%)  
  IF index% = -1 THEN
    ObjList_remove% = -1
    EXIT FUNCTION
  ENDIF

  LOCAL byteOffset%=index%*2
  LOCAL numBytes%=objList_numElems%(objList_id%)*2
  LOCAL bytesToDel%=2

  delBytesFromBlock objList_list%(0, objList_id%), byteOffset%, numBytes%, bytesToDel%

  INC objList_numElems%(objList_id%), -1

  ObjList_remove% = 0
END FUNCTION

SUB ObjList_clear(objList_id%)
 objList_numElems%(objList_id%) = 0
END SUB

FUNCTION ObjList_isEmpty%(objList_id%)
  ObjList_isEmpty% = objList_numElems%(objList_id%) = 0
END FUNCTION

FUNCTION ObjList_isFull%(objList_id%)
  ObjList_isFull% = objList_numElems%(objList_id%) >= MAX_OBJ_LIST_SIZE%
END FUNCTION

'Compare two objLists for equality
FUNCTION ObjList_equal%(objList_id1%, objList_id2%)
  IF objList_numElems%(objList_id1%) <> objList_numElems%(objList_id2%) THEN
    ObjList_equal% = 0
  ENDIF

  LOCAL numBytes% = objList_numElems%(objList_id1%)*2
  LOCAL res%
  
  blockCompareEQ(objList_list%(0, objList_id1%), objList_list%(0, objList_id2%), numBytes%ù, res%)

  ObjList_equal% = res%
END FUNCTION

SUB ObjList_append(to_objLid%, from_objLid%)
  LOCAL from_numBytes% = objList_numElems%(from_objLid%)*2
  LOCAL to_numBytes% = objList_numElems%(to_objLid%)*2
  LOCAL numBytes% = from_numBytes% + to_numBytes%
  LOCAL fromOffset% = 0
  
  IF numBytes% >= MAX_OBJ_LIST_SIZE%*8 THEN
    Error "Can't append to objList "+STR$(to_objLid%)
  ENDIF
  
  blockCopy(objList_list%(0, to_objLid%), to_numBytes%, objList_list%(0, from_objLid%), fromOffset%, from_numBytes%)

  objList_numElems%(to_objLid%) = numBytes%>>1
END SUB

