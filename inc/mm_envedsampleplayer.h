#ifndef MM_ENVEDSAMPLEPLAYER_H
#define MM_ENVEDSAMPLEPLAYER_H 

#include "mm_sigproc.h" 
#include "mm_sigchain.h"
#include "mm_sampleplayer.h" 
#include "mm_envelope.h"
#include "mm_enveloper.h" 
#include "mm_bus.h" 

typedef struct __MMEnvedSamplePlayer MMEnvedSamplePlayer;

struct __MMEnvedSamplePlayer {
    MMSigProc head;
    MMSigChain sigChain;
    MMSigConst sigConst;
    MMSamplePlayer sp;
    MMSamplerPlayerSigProc spsp;
    MMEnveloper enver;
    MMBusMerger bm;
    MMBus *internalBus;
};

void MMEnvedSamplePlayer_init(MMEnvedSamplePlayer *esp, MMEnvelope *env, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod);

#endif /* MM_ENVEDSAMPLEPLAYER_H */
