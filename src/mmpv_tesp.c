/* An poly voice for an trapezoid enveloped sample player */
/* This is sort of a proof-of-concept and will probably become obsolete when the
 * MMEnvedSamplePlayer is generalized for arbitrary envelopes */
#include "mmpv_tesp.h" 
#include "mm_poly_voice_manage.h" 
#include "mm_trapenvedsampleplayer.h" 

struct __MMPvtesp {
    MMPolyVoice head;
    MMTrapEnvedSamplePlayer *tesp;
};

typedef enum {
    MMPvtespParamType_NOTEON,   /* parameters for a note on */
    MMPvtespParamType_NOTEOFF,  /* parameters for a note off */
    MMPvtespParamType_ONDONE    /* parameters for when called as a callback when
                                   note finished decaying */
} MMPvtespParamType;


static void MMPvtesp_turnOn(MMPolyVoice *pv, void *params)
{
    MMTrapEnvedSamplePlayer *tesp = ((MMPvtesp*)pv)->tesp;
    MMPvtespParams *np = (MMPvtespParams*)params;
    /* Formally, the paramType should only be of type NOTEON but we're loose
     * about it and don't check */
    MMTrapEnvedSamplePlayer_noteOn(tesp, np->interpolation, np->index, np->note,
            np->amplitude, np->attackTime, np->releaseTime, np->samples);
}

static void MMPvtesp_turnOff(MMPolyVoice *pv, void *params)
{
    MMTrapEnvedSamplePlayer *tesp = ((MMPvtesp*)pv)->tesp;
    MMPvtespParams *np = (MMPvtespParams*)params;
    /* Currently there's no difference between how this behaves whether the
     * parameter type is NOTEON on NOTEOFF we just use the releaseTime. The only
     * difference is that for NOTEON, the release time should probably be
     * shorter (but this function is not responsible for this) */
    MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp).releaseTime = params->releaseTime;
    MMEnvelope_startRelease(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}

/* A way to trigger another note following this note's release */
static void MMPvtesp_onDone(MMEnvedSamplePlayer *esp, void *params)
{
    MMTrapEnvedSamplePlayer *tesp = (MMTrapEnvedSamplePlayer*)esp;
    MMPvtespParams *np = (MMPvtespParams*)params;
    MMTrapEnvedSamplePlayer_noteOn(tesp, np->interpolation, np->index, np->note,
            np->amplitude, np->attackTime, np->releaseTime, np->samples);
    esp->onDone = NULL;
    esp->onDoneParams = NULL;
}

static void MMPvtesp_attachOnTurnOff(MMPolyVoice *pv, void *params)
{
    MMEnvedSamplePlayer *esp = (MMEnvedSamplePlayer*)((MMPvtesp*)pv)->tesp;
    MMPvtespParams *np = (MMPvtespParams*)params;
    /* If called with NOTEON parameters, this means we want to trigger a note
     * after the current note has been turned off because it has been stolen */
    if (np->paramType == MMPvtespParamType_NOTEON) {
        esp->onDone = MMPvtesp_onDone;
        np->paramType = MMPvtespParamType_ONDONE;
        esp->onDoneParams = params;
    }
}

static int MMPvtesp_compare(MMPolyVoice *pv, void *params)
{
    MMTrapEnvedSamplePlayer *tesp = ((MMPvtesp*)pv)->tesp;
    MMPvtespParams *np = (MMPvtespParams*)params;
    if (MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).note == np->note) {
        return 0;
    }
    return 1;
}

static int MMPvtesp_init(MMPvtesp *pvtesp, MMTrapEnvedSamplePlayer *tesp)
{
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
