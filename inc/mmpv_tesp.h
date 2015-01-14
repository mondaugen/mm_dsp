#ifndef MMPV_TESP_H
#define MMPV_TESP_H 

#include "mm_sample.h" 
#include "mm_wavtab.h" 
#include "mm_trapenvedsampleplayer.h" 

typedef struct __MMPvtesp MMPvtesp;

typedef struct __MMPvtespParams MMPvtespParams;

typedef enum {
    MMPvtespParamType_NOTEON,   /* parameters for a note on */
    MMPvtespParamType_NOTEOFF,  /* parameters for a note off */
    MMPvtespParamType_ONDONE    /* parameters for when called as a callback when
                                   note finished decaying */
} MMPvtespParamType;

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
    MMWavTab          *samples;
};

MMPvtesp *MMPvtesp_new(MMTrapEnvedSamplePlayer *tesp);

#endif /* MMPV_TESP_H */
