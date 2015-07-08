#ifndef MMPV_TESP_H
#define MMPV_TESP_H 

#include "mm_sample.h" 
#include "mm_wavtab.h" 
#include "mm_trapenvedsampleplayer.h" 
#include "mm_poly_voice_manage.h" 

typedef struct __MMPvtesp MMPvtesp;

typedef struct __MMPvtespParams MMPvtespParams;

typedef enum {
    MMPvtespParamType_NOTEON,   /* parameters for a note on */
    MMPvtespParamType_NOTEOFF,  /* parameters for a note off */
    MMPvtespParamType_ONDONE    /* parameters for when called as a callback when
                                   note finished decaying */
} MMPvtespParamType;

typedef enum {
    MMPvtespRateSource_NOTE,    /* Note will be used to set the rate of the
                                   playback (interpreted as a midi note). Rate is ignored. */
    MMPvtespRateSource_RATE     /* Rate will be used to set the rate of playback
                                   (1.0 is normal speed for example). Note still
                                   used to keep track of the "voice" */
} MMPvtespRateSource;

/* On a note on you can indicate note (MIDI note where 60 = middle C, but can be
 * of type MMSample), amplitude (MMSample), interpolation (one of
 * MMInterpMethod), starting index (samples), attack time (seconds), release
 * time (seconds, can be changed later before the note off, so it is not
 * critical here), samples (pointer to WavTav) */
struct __MMPvtespParams {
    MMPolyVoiceParams   head;
    MMPvtespParamType   paramType;
    MMPvtespRateSource  rateSource;
    MMTrapEnvedSamplePlayer_noteOnStruct noteOnParams;
};

MMPvtesp *MMPvtesp_new(MMTrapEnvedSamplePlayer *tesp);

MMPvtespParams *MMPvtespParams_new();

#endif /* MMPV_TESP_H */
