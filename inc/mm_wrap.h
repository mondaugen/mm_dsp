#ifndef MM_WRAP_H
#define MM_WRAP_H 
#include <stdint.h> 
#include <math.h> 
#include "mm_sample.h" 

/*  wrap the value of x so it stays in interval [a,b) */
int MM_wrap(int x, int a, int b);
int64_t MM_wrapl(int64_t x, int64_t a, int64_t b);
MMSample MM_fwrap(MMSample x, MMSample a, MMSample b);

#endif /* MM_WRAP_H */
