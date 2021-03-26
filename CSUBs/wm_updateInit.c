#include "ARMCFunctions.h"
#include "df_defines.h"

/*IMPORTANT: This function uses CFuncRAM entry 7*/

void wm_updateInit(long long *obj_velocityp) {
	CFuncRam[CFUNC_RAM_IDX_OBJ_VELOCITY] = (int)obj_velocityp;
}
