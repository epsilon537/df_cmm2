#include "ARMCFunctions.h"
#include "df_defines.h"

/*IMPORTANT: This function uses CFuncRAM entries 0-6*/

void wm_draw_csub(long long *visible_objects_lids, long long *wm_viewp, char *callout_sub, long long *callout_arg) {
    long long *box_cornerp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER];
    long long *box_horizontalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_HORIZONTAL];
	long long *box_verticalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_VERTICAL];
	long long *objList_numElemsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJLIST_NUMELEMS];
	long long *objList_listp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJLIST_LIST];
	long long *obj_flagsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJ_FLAGS];
    long long *obj_positionp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION];

	long long wm_view = *wm_viewp;
	short int current_object;
	long long current_box;

	long long *visibile_objListp;
	long long visible_objects_lid;
	long long bx1 = box_cornerp[wm_view]>>48;
	long long by1 = (box_cornerp[wm_view]<<32)>>48;
	long long ax1, ay1;
	int xres, yres;
	long long wbox_corner;
	long long obj_flags;

	/*According to altitude*/
    for (int alt=0; alt<=MAX_ALTITUDE; alt++) {
    	visible_objects_lid = visible_objects_lids[alt];
    	visibile_objListp = (long long*)&objList_listp[MAX_OBJ_LIST_SIZE*visible_objects_lids[alt]];
    	for (int index=0; index<objList_numElemsp[visible_objects_lid]; index++) {
    		current_object = (short unsigned)(visibile_objListp[index>>2] >> (16*(index&3)));
    		obj_flags = obj_flagsp[current_object];

			/*Bounding boxes are relative to object, so convert to world coorindates*/
			current_box = (obj_flags & OBJ_FLG_BOX_MSK) >> OBJ_FLG_BOX_SHFT;
			wbox_corner = box_cornerp[current_box] + obj_positionp[current_object];

			/*Only draw if Object would be visible on window*/
			ax1 = wbox_corner>>48;
			ay1 = (wbox_corner<<32)>>48;
			
		    /*Test horizontal overlap*/
		    xres = ((bx1<=ax1) && (ax1 <= bx1 + box_horizontalp[wm_view])) || ((ax1<=bx1) && (bx1 <= ax1 + box_horizontalp[current_box]));
		    /*Test vertical overlap*/
		    yres = ((by1<=ay1) && (ay1 <= by1 + box_verticalp[wm_view])) || ((ay1<=by1) && (by1 <= ay1 + box_verticalp[current_box]));

			if ((xres && yres) || 
				((obj_flags & OBJ_FLG_TYPE_MSK) == (OBJ_TYPE_VIEW_OBJECT<<OBJ_FLG_TYPE_SHFT))) {
				*callout_arg = current_object;
				RunBasicSub(callout_sub+1);
			}
    		
    	}
    }
}