#ifndef MM_SAMPLE_H
#define MM_SAMPLE_H 

#include <stdint.h> 

typedef float MMSample;

typedef int32_t mm_q8_24_t;

#define mm_q8_24_t_from_MMSample(f) \
    ((MMSample)(f) * ((MMSample)(1 << 24L)))

#ifdef CHECK_SAMPLE_IS_FINITE
#include <math.h> 
#include <assert.h> 
static inline void MMSample_assert_is_finite(MMSample x)
{
    assert(isfinite(x));
}
#endif

#endif /* MM_SAMPLE_H */
