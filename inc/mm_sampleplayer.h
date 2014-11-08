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
    MMSamplePlayer *parent; /* where it gets its placeHolder and outBusses from */
    MMBool            loop;
    MMInterpMethod  interp;
};

#define MMSamplePlayerSigProc_free(spsp) free(spsp) 

MMSamplePlayerSigProc *MMSamplePlayerSigProc_new(void);
MMSamplePlayer *MMSamplePlayer_new(void);
void MMSamplePlayerSigProc_init(MMSamplePlayerSigProc *spsp);
void MMSamplePlayer_init(MMSamplePlayer *sp);

/* Functions for getting samples with vairous interpolations */
/* spsp is a pointer to a MMSamplePlayerSigProc and pdest is a pointer to an
 * MMSample */

#define MMSamplePlayerSigProc_getSampleInterpNone_(spsp,pdest) \
    do { \
        *(pdest) += MMWavTab_get(*((spsp)->samples),(spsp)->index); \
    } while (0)

#define MMSamplePlayerSigProc_getSampleInterpLinear_(spsp,pdest) \
    do { \
        int __x0 = (int)(spsp)->index; \
        int __x1 = MM_wrap((int)(spsp)->index + 1,\
            0, (int)(spsp)->samples->length); \
        *(pdest) += MM_f_interp_linear_(__x0, \
                MMWavTab_get(*((spsp)->samples),__x0), \
                __x1, \
                MMWavTab_get(*((spsp)->samples), __x1), \
                (spsp)->index); \
    } while (0)
    
#define MMSamplePlayerSigProc_getSampleInterpCubic_(spsp,pdest) \
    do { \
        int __x0 = MM_wrap((int)(spsp)->index - 1, 0, (int)(spsp)->samples->length); \
        int __x1 = MM_wrap((int)(spsp)->index    , 0, (int)(spsp)->samples->length); \
        int __x2 = MM_wrap((int)(spsp)->index + 1, 0, (int)(spsp)->samples->length); \
        int __x3 = MM_wrap((int)(spsp)->index + 2, 0, (int)(spsp)->samples->length); \
        *(pdest) += MM_f_interp_cubic_(__x0, \
                MMWavTab_get(*((spsp)->samples),__x0), \
                __x1, \
                MMWavTab_get(*((spsp)->samples), __x1), \
                __x2, \
                MMWavTab_get(*((spsp)->samples), __x2), \
                __x3, \
                MMWavTab_get(*((spsp)->samples), __x3), \
                (spsp)->index); \
    } while (0)
#define MMSamplePlayerSigProc_getSampleInterpCubicNorm_(spsp,pdest) \
    do { \
        int __x0 = MM_wrap((int)(spsp)->index - 1, 0, (int)(spsp)->samples->length); \
        int __x1 = MM_wrap((int)(spsp)->index    , 0, (int)(spsp)->samples->length); \
        int __x2 = MM_wrap((int)(spsp)->index + 1, 0, (int)(spsp)->samples->length); \
        int __x3 = MM_wrap((int)(spsp)->index + 2, 0, (int)(spsp)->samples->length); \
        *(pdest) += MM_f_interp_cubic_norm_( \
                MMWavTab_get(*((spsp)->samples),__x0), \
                MMWavTab_get(*((spsp)->samples), __x1), \
                MMWavTab_get(*((spsp)->samples), __x2), \
                MMWavTab_get(*((spsp)->samples), __x3), \
                (spsp)->index); \
    } while (0)

#endif /* MM_SAMPLEPLAYER_H */
