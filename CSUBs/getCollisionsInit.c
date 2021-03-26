#include "ARMCFunctions.h"
#include "df_defines.h"
/*IMPORTANT: This function uses CFuncRAM entries 3-6*/

void getCollisionsInit(long long* objList_numElemsp, long long* objListp, long long* obj_flagsp, long long* obj_positionp) {
 	CFuncRam[CFUNC_RAM_IDX_OBJLIST_NUMELEMS] = (int)objList_numElemsp;
	CFuncRam[CFUNC_RAM_IDX_OBJLIST_LIST] = (int)objListp;
	CFuncRam[CFUNC_RAM_IDX_OBJ_FLAGS] = (int)obj_flagsp;
    CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION] = (int)obj_positionp;
}