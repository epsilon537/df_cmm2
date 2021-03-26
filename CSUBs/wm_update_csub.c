#include "ARMCFunctions.h"
#include "df_defines.h"

void wm_update_csub(long long *active_objects_lidp, char *callout_sub, long long *callout_arg1, long long *callout_arg2) {
	long long *objList_numElemsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJLIST_NUMELEMS];
	long long *objList_listp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJLIST_LIST];
    long long *obj_positionp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION];
    long long *obj_velocityp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_VELOCITY];

	short int current_object;

	long long active_objects_lid = *active_objects_lidp;
	long long *active_objListp = (long long*)&objList_listp[MAX_OBJ_LIST_SIZE*active_objects_lid];

 	for (int index=0; index<objList_numElemsp[active_objects_lid]; index++) {
		current_object = (short unsigned)(active_objListp[index>>2] >> (16*(index&3)));
		//If position changed, move
		if (obj_velocityp[current_object]) {
			*callout_arg1 = current_object;
			*callout_arg2 = obj_positionp[current_object] + obj_velocityp[current_object];
			RunBasicSub(callout_sub+1);
		}
 	}
}