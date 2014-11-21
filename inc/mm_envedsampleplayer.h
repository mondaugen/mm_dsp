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
    MMBus *internalBus;
    MMBus *outBus;
};

void MMEnvedSamplePlayer_init(MMEnvedSamplePlayer *esp, MMEnvelope *env, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod)
{
    MMSigProc_init(esp);
    MMSigChain_init(&esp->sigChain);
    /* internal bus is only 1 channel */
    esp->internalBus = MMBus_new(internalBusSize,1); 
    /* Init enveloper */
    MMEnveloper_init(&esp->enver, env, esp->internalBus, tickPeriod);
    /* Add to top of internal sig chain */
    MMSigConst_insertAfter(&esp->sigChain.sigProcs, &esp->enver);
    /* Init sample player */
    MMSamplePlayer_init(&esp->sp);
    esp->sp.outBus = &esp->internalBus; 
    /* Add placeholder to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->sp.placeHolder);
    /* zero signal at top of internal bus */
    MMSigConst_init(&esp->sigConst, esp->internalBus, 0, MMSigConst_doSum_FALSE);
    /* Add to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->sigConst);
    /* Init sample player sig proc */
    MMSamplePlayerSigProc_init(&esp->spsp);
    esp->spsp.parent = &esp->spsp;
    MMSigProc_setState((&esp->spsp),MMSigProc_State_DONE);
    /* Insert at sampleplayer place holder */
    MMSigProc_insertAfter(&esp->sp.placeHolder, &esp->spsp);



#endif /* MM_ENVEDSAMPLEPLAYER_H */
