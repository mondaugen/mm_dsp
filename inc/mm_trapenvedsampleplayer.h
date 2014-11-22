#ifndef MM_TRAPENVEDSAMPLEPLAYER_H
#define MM_TRAPENVEDSAMPLEPLAYER_H 

#include "mm_envedsampleplayer.h"
#include "mm_trapezoid_env.h" 

typedef struct __MMTrapEnvedSamplePlayer MMTrapEnvedSamplePlayer;

struct __MMTrapEnvedSamplePlayer {
    MMEnvedSamplePlayer head;
    MMTrapezoidEnv te;
};

void MMTrapEnvedSamplePlayer_init(MMTrapEnvedSamplePlayer *tesp, MMBus *outBus,
        size_t internalBusSize, MMSample tickPeriod);

#define MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp) ((MMTrapEnvedSamplePlayer*)(tesp))->te

#endif /* MM_TRAPENVEDSAMPLEPLAYER_H */
