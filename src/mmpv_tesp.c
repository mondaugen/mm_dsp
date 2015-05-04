/* An poly voice for an trapezoid enveloped sample player */
/* This is sort of a proof-of-concept and will probably become obsolete when the
 * MMEnvedSamplePlayer is generalized for arbitrary envelopes */
#include "mmpv_tesp.h"

/* Choose noteOn method by checking the rate source. Using the sustain time is
 * not supported when using the poly_voice_manager. */
#define MMTrapEnvedSamplePlayer_noteOn_CHECK_RATE_SOURCE(tesp,np)\
    if (np->rateSource == MMPvtespRateSource_RATE) {\
        MMTrapEnvedSamplePlayer_noteOn_Rate(\
                tesp,\
                np->note,\
                np->amplitude,\
                np->interpolation,\
                np->index,\
                np->attackTime,\
                np->releaseTime,\
                -1,\
                np->samples,\
                np->loop,\
                np->rate);\
    } else {\
        MMTrapEnvedSamplePlayer_noteOn(\
                tesp,\
                np->note,\
                np->amplitude,\
                np->interpolation,\
                np->index,\
                np->attackTime,\
                np->releaseTime,\
                -1,\
                np->samples,\
                np->loop);\
    }

struct __MMPvtesp {
    MMPolyVoice head;
    MMTrapEnvedSamplePlayer *tesp;
};

static void MMPvtesp_turnOn(MMPolyVoice *pv, MMPolyVoiceParams *params)
{
    MMTrapEnvedSamplePlayer *tesp = ((MMPvtesp*)pv)->tesp;
    MMPvtespParams *np = (MMPvtespParams*)params;
    /* Formally, the paramType should only be of type NOTEON but we're loose
     * about it and don't check */
    MMTrapEnvedSamplePlayer_noteOn_CHECK_RATE_SOURCE(tesp,np);
    params->parent->used = MMPolyVoiceUsed_TRUE;
}

static void MMPvtesp_turnOff(MMPolyVoice *pv, MMPolyVoiceParams *params)
{
    MMTrapEnvedSamplePlayer *tesp = ((MMPvtesp*)pv)->tesp;
    MMPvtespParams *np = (MMPvtespParams*)params;
    /* Currently there's no difference between how this behaves whether the
     * parameter type is NOTEON on NOTEOFF we just use the releaseTime. The only
     * difference is that for NOTEON, the release time should probably be
     * shorter (but this function is not responsible for this) */
    if ((params->steal == MMPolyManagerSteal_FALSE) &&
            (MMEnvelope_getState(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp))
            == MMTrapezoidEnvState_RELEASE)) {
        /* If stealing is not allowed, and this note is already releasing, then
         * we don't want to retrigger the release as that would lengthen the
         * release and make it so the note is free (available) even later. */
        return; /* Already releasing */
    }
    MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp).releaseTime = np->releaseTime;
    MMEnvelope_startRelease(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}

/* A way to trigger another note following this note's release */
static void MMPvtesp_onDone(MMEnvedSamplePlayer *esp)
{
    MMTrapEnvedSamplePlayer *tesp = (MMTrapEnvedSamplePlayer*)esp;
    MMPvtespParams *np = (MMPvtespParams*)esp->onDoneParams;
    /* If there's an allocator, let it know that this note was
     * freed */
    ((MMPolyVoiceParams*)np)->allocator ?
        ((MMPolyVoiceParams*)np)->yield_params_to_allocator(
            ((MMPolyVoiceParams*)np)->allocator,
            /* the allocator yielding function should expect a note of type MMSample* */
            (void *)&(MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note))
        : 0;
    /* If params is from a NOTEON, that means the note was stolen, told to end,
     * ended, and now needs to be retriggered */
    if (np->paramType == MMPvtespParamType_NOTEON) {
        MMTrapEnvedSamplePlayer_noteOn_CHECK_RATE_SOURCE(tesp,np);
    } else {
        /* Mark note as NOT being played */
        ((MMPolyVoiceParams*)np)->parent->used = MMPolyVoiceUsed_FALSE;
        /* Remove from list of notes playing */
        MMDLList_remove((MMDLList*)((MMPolyVoiceParams*)np)->parent);
    }
    /* Free the params */
    free(esp->onDoneParams);
    esp->onDone = NULL;
    esp->onDoneParams = NULL;
}

static void MMPvtesp_attachOnTurnOff(MMPolyVoice *pv, MMPolyVoiceParams *params)
{
    MMEnvedSamplePlayer *esp = (MMEnvedSamplePlayer*)((MMPvtesp*)pv)->tesp;
//    MMPvtespParams *np = (MMPvtespParams*)params;
    /* If called with NOTEON parameters, this means we want to trigger a note
     * after the current note has been turned off because it has been stolen */
    /* if esp->onDoneParams not NULL it means this note is already waiting
     * to be done, which will now never happen for this set of params
     * because they are now going to be replaced, so free the params that
     * are there */
    if (esp->onDoneParams) {
        free(esp->onDoneParams);
    }
    esp->onDone = MMPvtesp_onDone;
    esp->onDoneParams = params;
}

static int MMPvtesp_compare(MMPolyVoice *pv, MMPolyVoiceParams *params)
{
    MMTrapEnvedSamplePlayer *tesp = ((MMPvtesp*)pv)->tesp;
    MMPvtespParams *np = (MMPvtespParams*)params;
    if (MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note == np->note) {
        return 0;
    }
    return 1;
}

static void MMPvtesp_init(MMPvtesp *pvtesp, MMTrapEnvedSamplePlayer *tesp)
{
    MMPolyVoice_init(pvtesp);
    MMPolyVoice_set_turnOn(pvtesp, MMPvtesp_turnOn);
    MMPolyVoice_set_turnOff(pvtesp, MMPvtesp_turnOff);
    MMPolyVoice_set_compare(pvtesp, MMPvtesp_compare);
    MMPolyVoice_set_attachOnTurnOff(pvtesp, MMPvtesp_attachOnTurnOff);
    MMPolyVoice_set_used(pvtesp, MMPolyVoiceUsed_FALSE);
    pvtesp->tesp = tesp;
};

MMPvtesp *MMPvtesp_new(MMTrapEnvedSamplePlayer *tesp)
{
    MMPvtesp *result = (MMPvtesp*)malloc(sizeof(MMPvtesp));
    if (result) {
        MMPvtesp_init(result,tesp);
    }
    return result;
}

MMPvtespParams *MMPvtespParams_new()
{
    MMPvtespParams *result = (MMPvtespParams*)malloc(sizeof(MMPvtespParams));
    MMPolyVoiceParams_init(result);
    result->paramType       = MMPvtespParamType_NOTEOFF;
    result->rateSource      = MMPvtespRateSource_NOTE;
    result->rate            = 0;
    result->note            = 0;
    result->amplitude       = 0;
    result->interpolation   = MMInterpMethod_NONE;
    result->index           = 0;
    result->attackTime      = 0;
    result->releaseTime     = 0;
    result->samples         = NULL;
    result->loop            = 0;
    return result;
}
