#ifndef MM_ENVEDSAMPLEPLAYER_H
#define MM_ENVEDSAMPLEPLAYER_H 

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
    MMSigProc head;
    MMSigChain sigChain;
    MMSigConst sigConst;
    MMSamplePlayer sp;
    MMSamplePlayerSigProc spsp;
    MMEnveloper enver;
    MMBusMerger bm;
    MMBus *internalBus;
    void (*onDone)(MMEnvedSamplePlayer*);
};

void MMEnvedSamplePlayer_init(MMEnvedSamplePlayer *esp, MMEnvelope *env, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod);

#define MMEnvedSamplePlayer_getSamplePlayerSigProc(esp) ((MMEnvedSamplePlayer*)(esp))->spsp
#define MMEnvedSamplePlayer_getEnveloper(esp) ((MMEnvedSamplePlayer*)(esp))->enver 
#define MMEnvedSamplePlayer_doOnDone(esp) \
    ((MMEnvedSamplePlayer*)esp)->onDone((MMEnvedSamplePlayer*)esp)

#endif /* MM_ENVEDSAMPLEPLAYER_H */
