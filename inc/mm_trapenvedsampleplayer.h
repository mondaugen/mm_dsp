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

typedef struct __MMTrapEnvedSamplePlayerInitStruct {
    MMEnvedSamplePlayerInitStruct head;
    MMSample tickPeriod;
} MMTrapEnvedSamplePlayerInitStruct;

void MMTrapEnvedSamplePlayer_init(MMTrapEnvedSamplePlayer *tesp,    
                                  MMTrapEnvedSamplePlayerInitStruct *init);

typedef struct __MMTrapEnvedSamplePlayer_noteOnStruct {
        MMSample        note;
        MMSample        amplitude;
        MMSample        index;
        MMSample        attackTime;
        MMSample        releaseTime;
        MMSample        sustainTime;
        MMWavTab        *samples;
        MMSample        rate;
        mm_q8_24_t      *p_rate;
} MMTrapEnvedSamplePlayer_noteOnStruct;

void MMTrapEnvedSamplePlayer_noteOn(
        MMTrapEnvedSamplePlayer *tesp,
        MMTrapEnvedSamplePlayer_noteOnStruct *init);

void MMTrapEnvedSamplePlayer_noteOn_Rate(
        MMTrapEnvedSamplePlayer *tesp,
        MMTrapEnvedSamplePlayer_noteOnStruct *init);

void MMTrapEnvedSamplePlayer_noteOn_pRate(
        MMTrapEnvedSamplePlayer *tesp,
        MMTrapEnvedSamplePlayer_noteOnStruct *init);

#define MMTrapEnvedSamplePlayer_getTrapezoidEnv(tesp)\
    ((MMTrapEnvedSamplePlayer*)(tesp))->teg.te

#endif /* MM_TRAPENVEDSAMPLEPLAYER_H */
