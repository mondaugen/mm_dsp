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
 * The init struct:
    MMEnvelope *env;
    MMBus *outBus;
    MMInterpMethod interp;
    MMEnvedSamplePlayerTickType tickType;
    uint32_t internalBusSize;
 * env : the Envelope that is enveloping the sample player. It needs to know
 *     the address of this so it can poll it's state to see if it's done to call
 *     the optional doneAction.
 * outBus :is the bus the final enveloped sample player signal will be written to
 * interp : can be one of MMInterpMethod and determines what interpolation the
 *        sample player will use to look up samples.
 * tickType : this can be one of MMEnvedSamplePlayerTickType.
 * 
 *
 *
 */
void MMEnvedSamplePlayer_init(MMEnvedSamplePlayer *esp,
                              MMEnvedSamplePlayerInitStruct *init)
{
    MMSigProc_init((MMSigProc*)esp);
    MMSigChain_init(&esp->sigChain);
    /* internal bus is only 1 channel.
     * If the signal is to be summed into the outBus, the internal bus is needed
     * so that the sample player's output can be multiplied by the enveloper
     * without affecting the outBus. */
    if (init->tickType == MMEnvedSamplePlayerTickType_SUM) {
        esp->internalBus = MMBus_new(init->internalBusSize,1);
    } else {
        esp->internalBus = NULL;
    }
    /* envelope bus is only 1 channel */
    esp->envBus = MMBus_new(init->internalBusSize,1); 
    /* If it is supposed to sum into outBus so that it mixes with whatever is
     * there, make a bus merger. */
    if (init->tickType == MMEnvedSamplePlayerTickType_SUM) {
        /* Init bus merger */
        MMBusMerger_init(&esp->bm, esp->internalBus, init->outBus);
        /* Add bus merger to top of internal sig chain */
        MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->bm);
    }
    /* Store envelope */
    esp->envelope = init->env;
    /* Init bus multiplyer.
     * If the signal is to be summed into the outBus, the enveloper must
     * multiply the internal bus, otherwise it can directly multiply the outBus.
     * */
    MMBusMult_init(&esp->busMult, 
            (init->tickType == MMEnvedSamplePlayerTickType_SUM) ?
                esp->internalBus : init->outBus,
            esp->envBus);
    /* Add bus multiplyer to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->busMult);
    /* Init sample player sig proc */
    MMSamplePlayerSigProcInitStruct spspis;
    /* If the signal is to be summed into the final bus, the sample player must
     * write to the internal bus, otherwise it can write directly to the outBus.
     * */
    spspis.outBus   = (init->tickType == MMEnvedSamplePlayerTickType_SUM) ?
                        esp->internalBus : init->outBus;
    spspis.samples  = NULL;
    spspis.interp   = init->interp;
    spspis.tickType = MMSamplePlayerTickType_NOSUM;
    MMSamplePlayerSigProc_init(&esp->spsp, &spspis);
    /* Add sample player sig proc to top of internal sig chain */
    MMSigProc_insertAfter(&esp->sigChain.sigProcs, &esp->spsp);
    /* Set tick method */
    MMSigProc_setTick(esp, MMEnvedSamplePlayer_tick);
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
