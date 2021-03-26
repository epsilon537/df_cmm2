#ifndef __DF_DEFINES_H__
#define _DF_DEFINES_H__

#define MAX_ALTITUDE 3
#define MAX_OBJ_LIST_SIZE 100
#define OBJ_FLG_ALT_MSK (15<<9)
#define OBJ_FLG_ALT_SHFT 9
#define OBJ_FLG_BOX_MSK (1023<<15)
#define OBJ_FLG_BOX_SHFT 15
#define OBJ_FLG_SOLID_MSK (3<<13)
#define OBJ_FLG_SOLID_SHFT 13
#define OBJ_FLG_TYPE_MSK (255<<1)
#define OBJ_FLG_TYPE_SHFT 1
#define OBJ_SOLID_SPECTRAL 2
#define MAX_NUM_OBJS_IN_LIST (MAX_OBJ_LIST_SIZE*4)
#define OBJ_TYPE_VIEW_OBJECT 1

#define CFUNC_RAM_IDX_BOX_CORNER 0
#define CFUNC_RAM_IDX_BOX_HORIZONTAL 1
#define CFUNC_RAM_IDX_BOX_VERTICAL 2
#define CFUNC_RAM_IDX_OBJLIST_NUMELEMS 3
#define CFUNC_RAM_IDX_OBJLIST_LIST 4
#define CFUNC_RAM_IDX_OBJ_FLAGS 5
#define CFUNC_RAM_IDX_OBJ_POSITION 6

#endif /*DF_DEFINES_H*/
