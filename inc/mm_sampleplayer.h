#ifndef MM_SAMPLEPLAYER_H
#define MM_SAMPLEPLAYER_H 

#include "mm_bool.h" 
#include "mm_sample.h"
#include "mm_bus.h"
#include "mm_array.h" 
#include "mm_sigproc.h" 
#include "mm_wavtab.h" 
#include "mm_wrap.h" 
#include "mm_interp.h" 

typedef struct __MMSamplePlayer MMSamplePlayer;

struct __MMSamplePlayer {
    /* only can output to one bus, this is not a problem (use a bus splitter..) */
    MMBus         *outBus;
    /* reference for where to put the playing samplers in the signal chain */
    MMSigProc placeHolder;
};

typedef struct __MMSamplePlayerSigProc MMSamplePlayerSigProc;

struct __MMSamplePlayerSigProc {
    MMSigProc         head; /* subclasses MMSigProc */
    MMWavTab      *samples;
    MMSample         index;
    MMSample          rate;
    MMSample          note; /* the midi note of this instance, because rate may
                               be modulated so we need something static to
                               compare to check if a particular note is sounding
                               */
    MMSamplePlayer *parent; /* where it gets its placeHolder and outBusses from */
    MMBool            loop;
    MMInterpMethod  interp;
};

typedef enum {
    MMSamplePlayerTickType_NOSUM,
    MMSamplePlayerTickType_SUM
} MMSamplePlayerTickType;

#define MMSamplePlayerSigProc_free(spsp) free(spsp) 

MMSamplePlayerSigProc *MMSamplePlayerSigProc_new(void);
MMSamplePlayer *MMSamplePlayer_new(void);
void MMSamplePlayerSigProc_init(MMSamplePlayerSigProc *spsp, MMSamplePlayerTickType tt);
void MMSamplePlayer_init(MMSamplePlayer *sp);

/* Functions for getting samples with vairous interpolations */
/* spsp is a pointer to a MMSamplePlayerSigProc and pdest is a pointer to an
 * MMSample */

#define MMSamplePlayerSigProc_getSampleInterpNone_(spsp,pdest) \
    do { \
        *(pdest) = MMWavTab_get(((spsp)->samples),(spsp)->index); \
    } while (0)

#define MMSamplePlayerSigProc_getSampleInterpNone_sum_(spsp,pdest) \
    do { \
        *(pdest) += MMWavTab_get(((spsp)->samples),(spsp)->index); \
    } while (0)

#define MMSamplePlayerSigProc_getSampleInterpLinear_(spsp,pdest) \
    do { \
        int __x0 = (int)(spsp)->index; \
        int __x1 = MM_wrap((int)(spsp)->index + 1, 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        *(pdest) = MM_f_interp_linear_msp_( \
                MMWavTab_get(((spsp)->samples),__x0), \
                MMWavTab_get(((spsp)->samples),__x1), \
                (spsp)->index - (int)((spsp)->index));\
    } while (0)

#define MMSamplePlayerSigProc_getSampleInterpLinear_sum_(spsp,pdest) \
    do { \
        int __x0 = (int)(spsp)->index; \
        int __x1 = MM_wrap((int)(spsp)->index + 1, 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        *(pdest) += MM_f_interp_linear_msp_( \
                MMWavTab_get(((spsp)->samples),__x0), \
                MMWavTab_get(((spsp)->samples),__x1), \
                (spsp)->index - (int)((spsp)->index));\
    } while (0)
    
/* This is slow and sucks */
#define MMSamplePlayerSigProc_getSampleInterpCubic_(spsp,pdest) \
    do { \
        int __x0 = MM_wrap((int)(spsp)->index - 1, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        int __x1 = MM_wrap((int)(spsp)->index    , 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x2 = MM_wrap((int)(spsp)->index + 1, 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x3 = MM_wrap((int)(spsp)->index + 2, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        *(pdest) += MM_f_interp_cubic_(__x0, \
                MMWavTab_get(((spsp)->samples),__x0), \
                __x1, \
                MMWavTab_get(((spsp)->samples), __x1), \
                __x2, \
                MMWavTab_get(((spsp)->samples), __x2), \
                __x3, \
                MMWavTab_get(((spsp)->samples), __x3), \
                (spsp)->index); \
    } while (0)

#define MMSamplePlayerSigProc_getSampleInterpCubicNorm_(spsp,pdest) \
    do { \
        int __x0 = MM_wrap((int)(spsp)->index - 1, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        int __x1 = MM_wrap((int)(spsp)->index    , 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x2 = MM_wrap((int)(spsp)->index + 1, 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x3 = MM_wrap((int)(spsp)->index + 2, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        *(pdest) += MM_f_interp_cubic_norm_( \
                MMWavTab_get(((spsp)->samples),__x0), \
                MMWavTab_get(((spsp)->samples), __x1), \
                MMWavTab_get(((spsp)->samples), __x2), \
                MMWavTab_get(((spsp)->samples), __x3), \
                (spsp)->index); \
    } while (0)

/* Uses the "mu" cubic interpolation */
#define MMSamplePlayerSigProc_getSampleInterpCubicMu_(spsp,pdest) \
    do { \
        int __x0 = MM_wrap((int)(spsp)->index - 1, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        int __x1 = MM_wrap((int)(spsp)->index    , 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x2 = MM_wrap((int)(spsp)->index + 1, 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x3 = MM_wrap((int)(spsp)->index + 2, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        MMSample __y0 = MMWavTab_get(((spsp)->samples),__x0); \
        MMSample __y1 = MMWavTab_get(((spsp)->samples),__x1); \
        MMSample __y2 = MMWavTab_get(((spsp)->samples),__x2); \
        MMSample __y3 = MMWavTab_get(((spsp)->samples),__x3); \
        *(pdest) = MM_f_interp_cubic_mu_(__y0, __y1, __y2, __y3, \
                (spsp)->index - (int)((spsp)->index)); \
    } while (0)

/* Uses the "mu" cubic interpolation */
#define MMSamplePlayerSigProc_getSampleInterpCubicMu_sum_(spsp,pdest) \
    do { \
        int __x0 = MM_wrap((int)(spsp)->index - 1, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        int __x1 = MM_wrap((int)(spsp)->index    , 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x2 = MM_wrap((int)(spsp)->index + 1, 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x3 = MM_wrap((int)(spsp)->index + 2, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        MMSample __y0 = MMWavTab_get(((spsp)->samples),__x0); \
        MMSample __y1 = MMWavTab_get(((spsp)->samples),__x1); \
        MMSample __y2 = MMWavTab_get(((spsp)->samples),__x2); \
        MMSample __y3 = MMWavTab_get(((spsp)->samples),__x3); \
        *(pdest) += MM_f_interp_cubic_mu_(__y0, __y1, __y2, __y3, \
                (spsp)->index - (int)((spsp)->index)); \
    } while (0)

/* Uses Miller Puckette's cubic interpolation */
#define MMSamplePlayerSigProc_getSampleInterpCubicMsp_(spsp,pdest) \
    do { \
        int __x0 = MM_wrap((int)(spsp)->index - 1, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        int __x1 = MM_wrap((int)(spsp)->index    , 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x2 = MM_wrap((int)(spsp)->index + 1, 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x3 = MM_wrap((int)(spsp)->index + 2, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        MMSample __y0 = MMWavTab_get(((spsp)->samples),__x0); \
        MMSample __y1 = MMWavTab_get(((spsp)->samples),__x1); \
        MMSample __y2 = MMWavTab_get(((spsp)->samples),__x2); \
        MMSample __y3 = MMWavTab_get(((spsp)->samples),__x3); \
        *(pdest) = MM_f_interp_cubic_msp_(__y0, __y1, __y2, __y3, \
                (spsp)->index - (int)((spsp)->index)); \
    } while (0)

/* Uses Miller Puckette's cubic interpolation */
#define MMSamplePlayerSigProc_getSampleInterpCubicMsp_sum_(spsp,pdest) \
    do { \
        int __x0 = MM_wrap((int)(spsp)->index - 1, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        int __x1 = MM_wrap((int)(spsp)->index    , 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x2 = MM_wrap((int)(spsp)->index + 1, 0,  \
                (int)MMArray_get_length(spsp->samples)); \
        int __x3 = MM_wrap((int)(spsp)->index + 2, 0, \
                (int)MMArray_get_length(spsp->samples)); \
        MMSample __y0 = MMWavTab_get(((spsp)->samples),__x0); \
        MMSample __y1 = MMWavTab_get(((spsp)->samples),__x1); \
        MMSample __y2 = MMWavTab_get(((spsp)->samples),__x2); \
        MMSample __y3 = MMWavTab_get(((spsp)->samples),__x3); \
        *(pdest) += MM_f_interp_cubic_msp_(__y0, __y1, __y2, __y3, \
                (spsp)->index - (int)((spsp)->index)); \
    } while (0)

#endif /* MM_SAMPLEPLAYER_H */
