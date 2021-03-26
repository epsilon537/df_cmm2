#include "ARMCFunctions.h"

/*IMPORTANT: This function uses CFuncRAM entries 0-2*/

void boxIntersectBoxInit(long long *box_cornerp, long long *box_horizontalp, long long *box_verticalp) {    
    CFuncRam[0] = (int)box_cornerp;
	CFuncRam[1] =(int)box_horizontalp;
	CFuncRam[2] = (int)box_verticalp;
}