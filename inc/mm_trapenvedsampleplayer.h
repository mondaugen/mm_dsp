#ifndef MM_TRAPENVEDSAMPLEPLAYER_H
#define MM_TRAPENVEDSAMPLEPLAYER_H 

#include "mm_wavtab.h" 
#include "mm_envedsampleplayer.h"
#include "mm_trapezoid_env.h" 
#include "mm_trapenvgen.h" 

typedef struct __MMTrapEnvedSamplePlayer MMTrapEnvedSamplePlayer;

struct __MMTrapEnvedSamplePlayer {
    MMEnvedSamplePlayer head;
    MMTrapEnvGen        teg;
};

void MMTrapEnvedSamplePlayer_init(MMTrapEnvedSamplePlayer *tesp, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod);

void MMTrapEnvedSamplePlayer_noteOn(
        MMTrapEnvedSamplePlayer *tesp,
        MMSample        note,
        MMSample        amplitude,
        MMInterpMethod  interpolation,
        MMSample        index,
        MMSample        attackTime,
        MMSample        releaseTime,
        MMWavTab        *samples,
        MMBool          loop);

#define MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp) ((MMTrapEnvedSamplePlayer*)(tesp))->teg.te

#endif /* MM_TRAPENVEDSAMPLEPLAYER_H */
