#include "mm_envedsampleplayer.h" 

static void MMEnvedSamplePlayer_onDone_default(MMEnvedSamplePlayer *esp)
{
    return;
}

static void MMEnvedSamplePlayer_tick(MMSigProc *esp)
{
    MMSigProc_defaultTick(esp);
    MMSigProc_tick(&((MMEnvedSamplePlayer*)esp)->sigChain);
    if (MMEnvedSamplePlayer_getEnvelope(esp)->state == MMEnvelopeState_OFF) {
        MMEnvedSamplePlayer_doOnDone(esp);
    }
}

/* This initializes an MMEnvedSamplePlayer, and it will know about the envelope
 * that is enveloping it, but it will not yet have a signal
 * being input into the multiplyer that that does the enveloping (it will just
 * multiply with garbage in the envBus). The MMEnvGen (or whatever) must be
 * added to the envBus and the sigChain by inheriting classes.
 */
void MMEnvedSamplePlayer_init(MMEnvedSamplePlayer *esp, MMEnvelope *env, MMBus *outBus,
        size_t internalBusSize)
{
    MMSigProc_init((MMSigProc*)esp);
    MMSigChain_init(&esp->sigChain);
    /* internal bus is only 1 channel */
    esp->internalBus = MMBus_new(internalBusSize,1); 
    /* envelope bus is only 1 channel */
    esp->envBus = MMBus_new(internalBusSize,1); 
    /* Init bus merger */
    MMBusMerger_init(&esp->bm, esp->internalBus, outBus);
    /* Add bus merger to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->bm);
    /* Store envelope */
    esp->envelope = env;
    /* Init bus multiplyer */
    MMBusMult_init(&esp->busMult, esp->internalBus, esp->envBus);
    /* Add bus multiplyer to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->busMult);
    /* Init sample player */
    MMSamplePlayer_init(&esp->sp);
    esp->sp.outBus = esp->internalBus; 
    /* Add sample player placeholder to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->sp.placeHolder);
    /* Init sample player sig proc */
    MMSamplePlayerSigProc_init(&esp->spsp, MMSamplePlayerTickType_SUM);
    esp->spsp.parent = &esp->sp;
    MMSigProc_setState((&esp->spsp),MMSigProc_State_DONE);
    /* Insert at sampleplayer place holder */
    MMSigProc_insertAfter(&esp->sp.placeHolder, &esp->spsp);
    MMSigProc_setTick(esp, MMEnvedSamplePlayer_tick);
    /* Initialize the sigConst that zeros the bus */
    MMSigConst_init(&esp->sigConst, esp->internalBus, 0, MMSigConst_doSum_FALSE);
    /* Insert at the top of the internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->sigConst);
    /* Set default on done action */
    esp->onDone = NULL;
    esp->onDoneParams = NULL;
    /* set initial state */
    MMSigProc_setState(
            &MMEnvedSamplePlayer_getSamplePlayerSigProc(esp),
            MMSigProc_State_DONE);
    MMSigProc_setDoneAction(
            &MMEnvedSamplePlayer_getSamplePlayerSigProc(esp),
            MMSigProc_DoneAction_NONE);
}
