#ifndef MM_ENVEDSAMPLEPLAYER_H
#define MM_ENVEDSAMPLEPLAYER_H 

#include <stddef.h> 
#include "mm_sigproc.h" 
#include "mm_sigchain.h"
#include "mm_sampleplayer.h" 
#include "mm_envelope.h"
#include "mm_enveloper.h" 
#include "mm_bus.h" 
#include "mm_sigconst.h" 
#include "mm_busmerger.h" 

typedef struct __MMEnvedSamplePlayer MMEnvedSamplePlayer;

struct __MMEnvedSamplePlayer {
    MMSigProc               head;
    MMSigChain              sigChain;
    MMSigConst              sigConst;
    MMSamplePlayer          sp;
    MMSamplePlayerSigProc   spsp;
    MMEnvelope              *envelope;
    MMBusMerger             bm;
    MMBusMult               busMult;
    MMBus                   *internalBus;
    MMBus                   *envBus;
    void                    (*onDone)(MMEnvedSamplePlayer*);
    void                    *onDoneParams;
};

void MMEnvedSamplePlayer_init(MMEnvedSamplePlayer *esp, MMEnvelope *env, MMBus *outBus,
        size_t internalBusSize);

#define MMEnvedSamplePlayer_getSamplePlayerSigProc(esp) ((MMEnvedSamplePlayer*)(esp))->spsp
#define MMEnvedSamplePlayer_getEnvelope(esp) ((MMEnvedSamplePlayer*)(esp))->envelope
#define MMEnvedSamplePlayer_doOnDone(esp) \
    ((MMEnvedSamplePlayer*)esp)->onDone ? \
        ((MMEnvedSamplePlayer*)esp)->onDone((MMEnvedSamplePlayer*)esp) : 0
/* Add the sigProc generating the enveloping signal, to be called by subclasses.
 * Adds the sigProc to the proper position in the sigChain. Inheriting classes
 * must tell envGen (the sigProc generating the enveloping signal) to write to
 * the internal envBus. */
#define MMEnvedSamplePlayer_insertEnvGen(esp,eg) \
    MMSigProc_insertBefore(((MMEnvedSamplePlayer*)esp)->busMult)

#endif /* MM_ENVEDSAMPLEPLAYER_H */
