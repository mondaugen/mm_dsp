#ifndef MM_WRAP_H
#define MM_WRAP_H 
#include <math.h> 
#include "mm_sample.h" 

/*  wrap the value of x so it stays in interval [a,b) */
MMSample MM_fwrap(MMSample x, MMSample a, MMSample b);

#endif /* MM_WRAP_H */
