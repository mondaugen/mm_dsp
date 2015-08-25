#ifndef MM_SAMPLE_H
#define MM_SAMPLE_H 

typedef float MMSample;

#ifdef CHECK_SAMPLE_IS_FINITE
#include <math.h> 
#include <assert.h> 
static inline void MMSample_assert_is_finite(MMSample x)
{
    assert(isfinite(x));
}
#endif

#endif /* MM_SAMPLE_H */
