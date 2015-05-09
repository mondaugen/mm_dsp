#ifndef MM_WAVTAB_H
#define MM_WAVTAB_H 
#include "mm_array.h" 
#include "mm_sample.h" 
#include "mm_interp.h" 
#include <stdint.h> 

/* Just the same as an MMArray, but must contain Sample type only! */
typedef struct __MMWavTab MMWavTab;
struct __MMWavTab {
    MMArray head;
    uint32_t samplerate; /* The sample rate that the sound in the wavetable was
                            "recorded" at*/
};

/* Table lookup, no interpolation. dest is a pointer to a Sample type. Wavtab is
 * instance of MMWavTab */
#define MMWavTab_get(wavtab,index) MMArray_get(wavtab,MMSample,(size_t)index)
/* Gets the oscillation frequency of the wavetable by dividing its samplerate by
 * its length */
#define MMWavTab_get_freq(w) ((MMSample)((MMWavTab*)w)->samplerate \
        / (MMSample)MMArray_get_length(w))

/* Get a value at a fractional index using a floating-point type and linearly
 * interpolating between two samples. This method is not recommended unless then
 * wave table is very short (under about 90000 samples for single-precision
 * float).*/
#define MMWavTab_get_interpLinear_flt_(wavtab,pdest,index) \
    do { \
        int __x0 = (int)(index); \
        int __x1 = MM_wrap((int)(index) + 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        *(pdest) = MM_f_interp_linear_msp_( \
                MMWavTab_get((wavtab),__x0), \
                MMWavTab_get((wavtab),__x1), \
                (index) - (int)(index));\
    } while (0)

/* Same as the function above, but sums the value into the variable */
#define MMWavTab_get_interpLinear_flt_sum_(wavtab,pdest,index) \
    do { \
        int __x0 = (int)(index); \
        int __x1 = MM_wrap((int)(index) + 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        *(pdest) += MM_f_interp_linear_msp_( \
                MMWavTab_get((wavtab),__x0), \
                MMWavTab_get((wavtab),__x1), \
                (index) - (int)(index));\
    } while (0)

/* Get a value at a fractional index using a uint64_t where the 32 MSB are the
 * integer part and the 32 LSB are the fractional part. This is the recommended
 * method. This method is not correct if "index" is negative. */
#define MMWavTab_get_interpLinear_q_32_32_(wavtab,pdest,index) \
    do { \
        int __x0 = (index >> 32); \
        int __x1 = MM_wrap(__x0 + 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        *(pdest) = MM_f_interp_linear_msp_( \
                MMWavTab_get((wavtab),__x0), \
                MMWavTab_get((wavtab),__x1), \
                (((MMSample)(0xffffffff & (index)))/(1l << 32)));\
    } while (0)

/* Same as above but sums into the variable *pdest */
#define MMWavTab_get_interpLinear_q_32_32_sum_(wavtab,pdest,index) \
    do { \
        int __x0 = (index >> 32); \
        int __x1 = MM_wrap(__x0 + 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        *(pdest) += MM_f_interp_linear_msp_( \
                MMWavTab_get((wavtab),__x0), \
                MMWavTab_get((wavtab),__x1), \
                (((MMSample)(0xffffffff & (index)))/(1l << 32)));\
    } while (0)


/* Similar to the linear interpolation routines for floating-point indices but
 * instead uses cubic interpolation from M. Puckette "Theory and Technique of
 * Electronic Music", p. 46, 2006. */
#define MMWavTab_get_interpCubic_flt_(wavtab,pdest,index) \
    do { \
        int __x0 = MM_wrap((int)(index) - 1, 0, \
                (int)MMArray_get_length((wavtab))); \
        int __x1 = MM_wrap((int)(index)    , 0,  \
                (int)MMArray_get_length((wavtab))); \
        int __x2 = MM_wrap((int)(index) + 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        int __x3 = MM_wrap((int)(index) + 2, 0, \
                (int)MMArray_get_length((wavtab))); \
        MMSample __y0 = MMWavTab_get((wavtab),__x0); \
        MMSample __y1 = MMWavTab_get((wavtab),__x1); \
        MMSample __y2 = MMWavTab_get((wavtab),__x2); \
        MMSample __y3 = MMWavTab_get((wavtab),__x3); \
        *(pdest) = MM_f_interp_cubic_msp_(__y0, __y1, __y2, __y3, \
                (index) - (int)(index)); \
    } while (0)

#define MMWavTab_get_interpCubic_flt_sum_(wavtab,pdest,index) \
    do { \
        int __x0 = MM_wrap((int)(index) - 1, 0, \
                (int)MMArray_get_length((wavtab))); \
        int __x1 = MM_wrap((int)(index)    , 0,  \
                (int)MMArray_get_length((wavtab))); \
        int __x2 = MM_wrap((int)(index) + 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        int __x3 = MM_wrap((int)(index) + 2, 0, \
                (int)MMArray_get_length((wavtab))); \
        MMSample __y0 = MMWavTab_get((wavtab),__x0); \
        MMSample __y1 = MMWavTab_get((wavtab),__x1); \
        MMSample __y2 = MMWavTab_get((wavtab),__x2); \
        MMSample __y3 = MMWavTab_get((wavtab),__x3); \
        *(pdest) += MM_f_interp_cubic_msp_(__y0, __y1, __y2, __y3, \
                (index) - (int)(index)); \
    } while (0)

/* Get a value at a fractional index using a uint64_t where the 32 MSB are the
 * integer part and the 32 LSB are the fractional part. This is the recommended
 * method. This method is not correct if "index" is negative. */
#define MMWavTab_get_interpCubic_q_32_32_(wavtab,pdest,index) \
    do { \
        int __x1 = (index >> 32); \
        int __x0 = MM_wrap(__x1 - 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        int __x2 = MM_wrap(__x1 + 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        int __x3 = MM_wrap(__x1 + 2, 0,  \
                (int)MMArray_get_length((wavtab))); \
        MMSample __y0 = MMWavTab_get((wavtab),__x0); \
        MMSample __y1 = MMWavTab_get((wavtab),__x1); \
        MMSample __y2 = MMWavTab_get((wavtab),__x2); \
        MMSample __y3 = MMWavTab_get((wavtab),__x3); \
        *(pdest) = MM_f_interp_cubic_msp_(__y0, __y1, __y2, __y3, \
                (((MMSample)(0xffffffff & (index)))/(1l << 32))); \
    } while (0)

#define MMWavTab_get_interpCubic_q_32_32_sum_(wavtab,pdest,index) \
    do { \
        int __x1 = (index >> 32); \
        int __x0 = MM_wrap(__x1 - 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        int __x2 = MM_wrap(__x1 + 1, 0,  \
                (int)MMArray_get_length((wavtab))); \
        int __x3 = MM_wrap(__x1 + 2, 0,  \
                (int)MMArray_get_length((wavtab))); \
        MMSample __y0 = MMWavTab_get((wavtab),__x0); \
        MMSample __y1 = MMWavTab_get((wavtab),__x1); \
        MMSample __y2 = MMWavTab_get((wavtab),__x2); \
        MMSample __y3 = MMWavTab_get((wavtab),__x3); \
        *(pdest) += MM_f_interp_cubic_msp_(__y0, __y1, __y2, __y3, \
                (((MMSample)(0xffffffff & (index)))/(1l << 32))); \
    } while (0)

#endif /* MM_WAVTAB_H */
