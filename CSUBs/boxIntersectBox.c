#include "ARMCFunctions.h"

/*IMPORTANT: This function uses CFuncRAM entries 0-2*/

void boxIntersectBox(long long* box_a_p, long long *box_b_p, long long *resp) {
    long long box_a = *box_a_p;
    long long box_b = *box_b_p;

	long long *box_cornerp = (long long*)CFuncRam[0];
	long long *box_horizontalp = (long long*)CFuncRam[1];
	long long *box_verticalp = (long long*)CFuncRam[2];

	long long ax1 = box_cornerp[box_a]>>48;
	long long bx1 = box_cornerp[box_b]>>48;
	long long ay1 = (box_cornerp[box_a]<<32)>>48;
	long long by1 = (box_cornerp[box_b]<<32)>>48;
    
    /*Test horizontal overlap*/
    long long xres = ((bx1<=ax1) && (ax1 <= bx1 + box_horizontalp[box_b])) || ((ax1<=bx1) && (bx1 <= ax1 + box_horizontalp[box_a]));
    /*Test vertical overlap*/
    long long yres = ((by1<=ay1) && (ay1 <= by1 + box_verticalp[box_b])) || ((ay1<=by1) && (by1 <= ay1 + box_verticalp[box_a]));

    *resp = xres && yres;
}