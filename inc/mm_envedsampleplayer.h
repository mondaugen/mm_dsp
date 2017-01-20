#ifndef MM_ENVEDSAMPLEPLAYER_H
#define MM_ENVEDSAMPLEPLAYER_H 

#include <stddef.h> 
#include <stdint.h> 
#include "mm_sigproc.h" 
#include "mm_sigchain.h"
#include "mm_sampleplayer.h" 
#include "mm_envelope.h"
#include "mm_enveloper.h" 
#include "mm_bus.h" 
#include "mm_sigconst.h" 
#include "mm_busmerger.h" 
#include "mm_busmult.h"
#include "mm_busconstmult.h" 

typedef struct __MMEnvedSamplePlayer MMEnvedSamplePlayer;

struct __MMEnvedSamplePlayer {
    MMSigProc               head;
    MMSigChain              sigChain;
    MMSigConst              sigConst;
    MMSamplePlayerSigProc   spsp;
    MMEnvelope              *envelope;
    MMBusMerger             bm;
    MMBusMult               busMult;
    MMBus                   *internalBus;
    MMBus                   *envBus;
    MMBusConstMult          bcm;
    void                    (*onDone)(MMEnvedSamplePlayer*);
    void                    *onDoneParams;
};

typedef enum {
    MMEnvedSamplePlayerTickType_NOSUM,
    MMEnvedSamplePlayerTickType_SUM
} MMEnvedSamplePlayerTickType;

typedef struct __MMEnvedSamplePlayerInitStruct {
    MMEnvelope *env;
    MMBus *outBus;
    MMInterpMethod interp;
    MMEnvedSamplePlayerTickType tickType;
    uint32_t internalBusSize;
} MMEnvedSamplePlayerInitStruct;

void MMEnvedSamplePlayer_init(MMEnvedSamplePlayer *esp,
                              MMEnvedSamplePlayerInitStruct *init);

#define MMEnvedSamplePlayer_getSamplePlayerSigProc(esp)\
    ((MMEnvedSamplePlayer*)(esp))->spsp
#define MMEnvedSamplePlayer_getEnvelope(esp) ((MMEnvedSamplePlayer*)(esp))->envelope
#define MMEnvedSamplePlayer_doOnDone(esp) \
    ((MMEnvedSamplePlayer*)esp)->onDone ? \
        ((MMEnvedSamplePlayer*)esp)->onDone((MMEnvedSamplePlayer*)esp) : 0
/* Add the sigProc generating the enveloping signal, to be called by subclasses.
 * Adds the sigProc to the proper position in the sigChain. Inheriting classes
 * must tell envGen (the sigProc generating the enveloping signal) to write to
 * the internal envBus. */
#define MMEnvedSamplePlayer_insertEnvGen(esp,eg) \
    MMSigProc_insertBefore(&((MMEnvedSamplePlayer*)esp)->busMult,eg)

#endif /* MM_ENVEDSAMPLEPLAYER_H */
