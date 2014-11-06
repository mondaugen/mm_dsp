#ifndef MM_STDLIB_H
#define MM_STDLIB_H 
#include <math.h> 
#include "mm_sample.h" 

/* wraps sample value x between 0 and max */
static inline MMSample mm_fwrap(MMSample x, MMSample max)
{
    return (x < 0) ? (fmod(x,max) + max) : (fmod(x,max));
}

#endif /* MM_STDLIB_H */
