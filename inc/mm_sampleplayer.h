#ifndef MM_SAMPLEPLAYER_H
#define MM_SAMPLEPLAYER_H 

#include "mm_bool.h" 
#include "mm_sample.h"
#include "mm_bus.h"
#include "mm_array.h" 

typedef struct __MMSamplePlayer MMSamplePlayer;

struct __MMSamplePlayer {
    /* only can output to one bus, this is not a problem (use a bus splitter..) */
    MMBus          outBus;
    /* reference for where to put the playing samplers in the signal chain */
    MMSigProc placeHolder;
};

typedef struct __MMSamplePlayerSigProc MMSamplePlayerSigProc;

struct __MMSamplePlayerSigProc {
    MMSigProc         head; /* subclasses MMSigProc */
    MMWavTab      *samples;
    MMSample         index;
    MMSample          rate;
    MMSamplePlayer *parent; /* where it gets its placeHolder and outBusses from */
    MMBool            loop;
};

#define MMSamplePlayerSigProc_free(spsp) free(spsp) 

MMSamplePlayerSigProc *MMSamplePlayerSigProc_new(void);
MMSamplePlayer *MMSamplePlayer_new(void);
void MMSamplePlayerSigProc_init(MMSamplePlayerSigProc *spsp)
void MMSamplePlayer_init(MMSamplePlayer *sp);

#endif /* MM_SAMPLEPLAYER_H */
