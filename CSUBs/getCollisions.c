#include "ARMCFunctions.h"
#include "df_defines.h"
/*IMPORTANT: This function uses CFuncRAM entries 0-6*/

/*collisions_lid will be set to -1 on error*/
void getCollisions(long long *obj_idp, long long *wherep, long long *solid_objects_lidp, long long *collisions_lidp) {
    long long solid_objects_lid = *solid_objects_lidp;
    long long collisions_lid = *collisions_lidp;

    long long *box_cornerp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_CORNER];
    long long *box_horizontalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_HORIZONTAL];
	long long *box_verticalp = (long long*)CFuncRam[CFUNC_RAM_IDX_BOX_VERTICAL];
	long long *objList_numElemsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJLIST_NUMELEMS];
	long long *objList_listp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJLIST_LIST];
	long long *obj_flagsp = (long long*)CFuncRam[CFUNC_RAM_IDX_OBJ_FLAGS];
    long long *obj_positionp = (long long *)CFuncRam[CFUNC_RAM_IDX_OBJ_POSITION];

    long long *solid_objListp = (long long*)&objList_listp[MAX_OBJ_LIST_SIZE*solid_objects_lid];
    long long *collisions_objListp = (long long*)&objList_listp[MAX_OBJ_LIST_SIZE*collisions_lid];
    
    long long obj_id = *obj_idp;
	long long bx1, by1, xres, yres, cl_numElems, real_index, shift;
    long long other_wbox_corner, other_wbox_id;
    short unsigned other_obj_id;
    long long ol_numElems = objList_numElemsp[solid_objects_lid];

    long long wbox = ((obj_flagsp[obj_id]) & OBJ_FLG_BOX_MSK) >> OBJ_FLG_BOX_SHFT;
    long long wbox_corner = box_cornerp[wbox] + *wherep;

	long long ax1 = wbox_corner>>48;
	long long ay1 = (wbox_corner<<32)>>48;
	long long ax2 = ax1 + box_horizontalp[wbox];
	long long ay2 = ay1 + box_verticalp[wbox];

	/*Iterate through all objects*/
    for (int index=0; index < ol_numElems; ++index) {
    	other_obj_id = (short unsigned)(solid_objListp[index>>2] >> (16*(index&3)));

    	/*Do not consider self*/
    	if (other_obj_id == obj_id) {
    		continue;
    	}

		other_wbox_id = ((obj_flagsp[other_obj_id]) & OBJ_FLG_BOX_MSK) >> OBJ_FLG_BOX_SHFT;
        other_wbox_corner = box_cornerp[other_wbox_id] + obj_positionp[other_obj_id];
	
		bx1 = other_wbox_corner>>48;		
		by1 = (other_wbox_corner<<32)>>48;
	    
	    /*Test horizontal overlap*/
	    xres = ((bx1<=ax1) && (ax1 <= bx1 + box_horizontalp[other_wbox_id])) || ((ax1<=bx1) && (bx1<=ax2));
	    /*Test vertical overlap*/
	    yres = ((by1<=ay1) && (ay1 <= by1 + box_verticalp[other_wbox_id])) || ((ay1<=by1) && (by1<=ay2));

	    /*Overlapping location*/
	    if (xres && yres) {
	    	/*ObjList_insert(collision_list)*/
	    	cl_numElems = objList_numElemsp[collisions_lid];
			if (cl_numElems >= MAX_NUM_OBJS_IN_LIST) {
				*collisions_lidp = -1;
				return;
			}

			real_index = cl_numElems/4;
			shift = 16*(cl_numElems&3);

			collisions_objListp[real_index] &= ~(0xffff<<shift);
			collisions_objListp[real_index] |= (other_obj_id<<shift);

			++objList_numElemsp[collisions_lid];
	    }
    }
}