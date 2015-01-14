#include "mm_envedsampleplayer.h" 

static void MMEnvedSamplePlayer_onDone_default(MMEnvedSamplePlayer *esp) {
    return;
}

static MMSigProc_Err MMEnvedSamplePlayer_tick(MMSigProc *esp)
{
    /* Call superclass tick method */
    MMSigProc_Err result;
    if ((result = MMSigProc_defaultTick(esp)) != MMSigProc_Err_GOOD) {
        return result;
    }
    MMSigProc_tick(&((MMEnvedSamplePlayer*)esp)->sigChain);
    if (MMEnvedSamplePlayer_getEnveloper(esp).env->state == MMEnvelopeState_OFF) {
        MMEnvedSamplePlayer_doOnDone(esp);
    }
    return result;
}

void MMEnvedSamplePlayer_init(MMEnvedSamplePlayer *esp, MMEnvelope *env, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod)
{
    MMSigProc_init((MMSigProc*)esp);
    MMSigChain_init(&esp->sigChain);
    /* internal bus is only 1 channel */
    esp->internalBus = MMBus_new(internalBusSize,1); 
    /* Init bus merger */
    MMBusMerger_init(&esp->bm, esp->internalBus, outBus);
    /* Add bus merger to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->bm);
    /* Init enveloper */
    MMEnveloper_init(&esp->enver, env, esp->internalBus, tickPeriod);
    /* Add enveloper to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->enver);
    /* Init sample player */
    MMSamplePlayer_init(&esp->sp);
    esp->sp.outBus = esp->internalBus; 
    /* Add sample player placeholder to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->sp.placeHolder);
    /* zero signal at top of internal bus */
    MMSigConst_init(&esp->sigConst, esp->internalBus, 0, MMSigConst_doSum_FALSE);
    /* Add to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->sigConst);
    /* Init sample player sig proc */
    MMSamplePlayerSigProc_init(&esp->spsp);
    esp->spsp.parent = &esp->sp;
    MMSigProc_setState((&esp->spsp),MMSigProc_State_DONE);
    /* Insert at sampleplayer place holder */
    MMSigProc_insertAfter(&esp->sp.placeHolder, &esp->spsp);
    MMSigProc_setTick(esp, MMEnvedSamplePlayer_tick);
    /* Set default on done action */
    esp->onDone = NULL;
    esp->onDoneParams = NULL;
}
