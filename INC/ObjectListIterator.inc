'Requires ObjectList.inc

'ObjectListIterator is a value type object used to iterate over a given list

CONST ITER_INDEX_SHIFT% = 16
CONST OBJLID_MASK% = 65535

'Initialize iterator object (should be treated as opaque).
SUB ObjListIter_init(iter%, objLid%)
  iter% = objLid%
END SUB

'Set iterator to 1st item in list
SUB ObjListIter_first(iter%)
  iter% = iter% AND OBJLID_MASK%
END SUB

'Set iterator to next item in list
SUB ObjListIter_next(iter%)
  LOCAL it_index% = iter%>>ITER_INDEX_SHIFT%, it_objLid% = iter% AND OBJLID_MASK%
  IF it_index% < objList_numElems%(it_objLid%) THEN: iter% = it_objLid% OR ((it_index%+1)<<ITER_INDEX_SHIFT%): ENDIF
END SUB

'Return true if at end of list
FUNCTION ObjListIter_isDone%(iter%)
  ObjListIter_isDone% = ((iter%>>ITER_INDEX_SHIFT%) >= objList_numElems%(iter% AND OBJLID_MASK%))
END FUNCTION

'Return current object id. or -1 if done/empty
FUNCTION ObjListIter_currentObject%(iter%)
  LOCAL it_index% = iter%>>ITER_INDEX_SHIFT%, it_objLid% = iter% AND OBJLID_MASK%

  ObjListIter_currentObject% = -1  
  'IF index% < objList_numElems%(objLid%) THEN
  '  ObjListIter_currentObject% = ObjList_get%(objLid%, index%)
  'ENDIF  
  IF it_index% < objList_numElems%(it_objLid%) THEN: ObjListIter_currentObject% = (objList_list%(it_index%>>2, it_objLid%) >> (16*(it_index% AND 3))) AND 65535: ENDIF
END FUNCTION



