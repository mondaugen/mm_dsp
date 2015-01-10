/* An poly voice for an trapezoid enveloped sample player */
/* This is sort of a proof-of-concept and will probably become obsolete when the
 * MMEnvedSamplePlayer is generalized for arbitrary envelopes */
#include "mm_poly_voice_manage.h" 
#include "mm_trapenvedsampleplayer.h" 
#include "mm_sample.h" 

typedef struct __MMPvtesp MMPvtesp;

struct __MMPvtesp {
    MMPolyVoice head;
    MMEnvedSamplePlayer *tesp;
};

typedef enum {
    MMPvtespParamType_NOTEON,   /* parameters for a note on */
    MMPvtespParamType_NOTEOFF,  /* parameters for a note off */
    MMPvtespParamType_ONDONE,   /* parameters for when called as a callback when
                                   note finished decaying */
} MMPvtespParamType;



typedef struct __MMPvtespParams MMPvtespParams;

/* On a note on you can indicate note (MIDI note where 60 = middle C, but can be
 * of type MMSample), amplitude (MMSample), interpolation (one of
 * MMInterpMethod), starting index (samples), attack time (seconds), release
 * time (seconds, can be changed later before the note off, so it is not
 * critical here), samples (pointer to WavTav) */
struct __MMPvtespParams {
    MMPvtespParamType paramType;
    MMSample        note;
    MMSample        amplitude;
    MMInterpMethod  interpolation;
    MMSample        index;
    MMSample        attackTime;
    MMSample        releaseTime;
    WavTav          *samples;
};

void MMPvtesp_turnOn(MMPolyVoice *pv, void *params)
{
    MMTrapEnvedSamplePlayer *tesp = ((MMPvtesp*)pv)->tesp;
    MMPvtespNoteOnParams *np = (MMPvtespNoteOnParams*)params;
    /* Formally, the paramType should only be of type NOTEON but we're loose
     * about it and don't check */
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).interp =
        np->interpolation;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).index =
        np->index;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).rate = MMCC_MIDItoRate(np->note);
    MMSigProc_setState(
            &MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp),
            MMSigProc_State_PLAYING);
    MMTrapezoidEnv_init(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp),
        0, np->amplitude, np->attackTime, np->releaseTime);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(tesp).samples = np->samples;
    MMEnvelope_startAttack(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}

void MMPvtesp_turnOff(MMPolyVoice *pv, void *params)
{
    MMTrapEnvedSamplePlayer *tesp = ((MMPvtesp*)pv)->tesp;
    MMPvtespNoteOnParams *np = (MMPvtespNoteOnParams*)params;
    /* Currently there's no difference between how this behaves whether the
     * parameter type is NOTEON on NOTEOFF we just use the releaseTime. The only
     * difference is that for NOTEON, the release time should probably be
     * shorter (but this function is not responsible for this) */
    MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp).releaseTime = params->releaseTime;
    MMEnvelope_startRelease(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp));
}
