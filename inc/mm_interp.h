#ifndef MM_INTERP_H
#define MM_INTERP_H 

#include "mm_sample.h" 

typedef enum
{
    MMInterpMethod_NONE,
    MMInterpMethod_LINEAR,
    MMInterpMethod_CUBIC
} MMInterpMethod;

/* Linear interpolation. Make line from points (x1,y1) and (x2,y2) and look up
 * the value x on this line. */
#define MM_f_interp_linear_(x1,y1,x2,y2,x) (((((MMSample)(y2)) - ((MMSample)(y1)))\
            /(((MMSample)(x2)) - ((MMSample)(x1)))) * (((MMSample)(x))\
            - ((MMSample)(x1))) + ((MMSample)(y1)))

/* Linear interpolation but assume x1 = 0 and x2 = 1 */
#define MM_f_interp_linear_norm_(y1,y2,x) cm_flinterp_(0,y1,1,y2,x)

/* This doesn't work well, I don't know why yet (maybe never will, it's slow
 * anyways) */
#define MM_f_interp_cubic_(x0,y0,x1,y1,x2,y2,x3,y3,x) \
    ((((x - x1) * (x - x2) * (x - x3)) / ((x0 - x1) * (x0 - x2) * (x0 - x3))) * y0 + \
     (((x - x0) * (x - x2) * (x - x3)) / ((x1 - x0) * (x1 - x2) * (x1 - x3))) * y1 + \
     (((x - x0) * (x - x1) * (x - x3)) / ((x2 - x0) * (x2 - x1) * (x2 - x3))) * y2 + \
     (((x - x0) * (x - x1) * (x - x2)) / ((x3 - x0) * (x3 - x1) * (x3 - x1))) * y3) 

/* Cubic interpolation but assume spacing between x coordinates changes by
 * increments of 1. x must be positive. */
#define MM_f_interp_cubic_norm_(y0,y1,y2,y3,x) \
    (-(x - (MMSample)((int)x)) * ((x - (MMSample)((int)x)) - 1) * ((x - (MMSample)((int)x)) - 2) / 6. * y0 + \
    ((x - (MMSample)((int)x)) + 1) * ((x - (MMSample)((int)x)) - 1) * ((x - (MMSample)((int)x)) - 2) / 2. * y1 + \
    -((x - (MMSample)((int)x)) + 1) * (x - (MMSample)((int)x)) * ((x - (MMSample)((int)x)) - 2) / 2. * y2 + \
    ((x - (MMSample)((int)x)) + 1) * (x - (MMSample)((int)x)) * ((x - (MMSample)((int)x)) - 1) / 6. * y3)

/* Cubic interpolation as implemented on this site:
 * http://paulbourke.net/miscellaneous/interpolation/ */
#define MM_f_interp_cubic_mu_(y0,y1,y2,y3,mu) \
    (((y3)-(y2)-(y1)+(y1)) * (mu) * ((mu)*(mu)) \
     + ((y0)-(y1)-((y3)-(y2)-(y1)+(y1))) * ((mu)*(mu)) \
     + ((y2)-(y0)) * (mu) + ((y1)))

#endif /* MM_INTERP_H */
