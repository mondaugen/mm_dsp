#ifndef MM_INTERP_H
#define MM_INTERP_H 

#include "mm_sample.h" 

/* Linear interpolation. Make line from points (x1,y1) and (x2,y2) and look up
 * the value x on this line. */
#define MM_f_interp_linear_(x1,y1,x2,y2,x) (((((MMSample)(y2)) - ((MMSample)(y1)))\
            /(((MMSample)(x2)) - ((MMSample)(x1)))) * (((MMSample)(x))\
            - ((MMSample)(x1))) + ((MMSample)(y1)))

/* Linear interpolation but assume x1 = 0 and x2 = 1 */
#define MM_f_interp_linear_norm_(y1,y2,x) cm_flinterp_(0,y1,1,y2,x)

#define MM_f_interp_cubic_(x0,y0,x1,y1,x2,y2,x3,y3,x) \
    ((((x - x1) * (x - x2) * (x - x3)) / ((x0 - x1) * (x0 - x2) * (x0 - x3))) * y0 + \
     (((x - x0) * (x - x2) * (x - x3)) / ((x1 - x0) * (x1 - x2) * (x1 - x3))) * y1 + \
     (((x - x0) * (x - x1) * (x - x3)) / ((x2 - x0) * (x2 - x1) * (x2 - x3))) * y2 + \
     (((x - x0) * (x - x1) * (x - x2)) / ((x3 - x0) * (x3 - x1) * (x3 - x1))) * y3) 

#endif /* MM_INTERP_H */
