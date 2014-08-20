#ifndef MM_SAMPLEPLAYER_H
#define MM_SAMPLEPLAYER_H 

#include "mm_sample.h"
#include "mm_bus.h"
#include "mm_array.h" 

typedef struct __MMSamplePlayer MMSamplePlayer;

struct __MMSamplePlayer {
    MMArray     outBusses;
    /* reference for where to put the playing samplers in the signal chain */
    MMSigProc placeHolder;
};

typedef struct __MMSamplePlayerSigProc MMSamplePlayerSigProc;

struct __MMSamplePlayerSigProc {
    MMSigProc head; /* subclasses MMSigProc */
    MMWavTab *samples;
    Sample index;
    Sample rate;
    MMSamplePlayer *parent; /* where it gets its placeHolder and outBusses from */
};

#endif /* MM_SAMPLEPLAYER_H */
