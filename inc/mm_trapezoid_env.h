#ifndef MM_TRAPEZOID_ENV_H
#define MM_TRAPEZOID_ENV_H 
#include "mm_sample.h" 
#include "mm_rationalfunc.h"

typedef enum {
    MMTrapezoidEnvState_OFF,
    MMTrapezoidEnvState_ATTACK,
    MMTrapezoidEnvState_SUSTAIN,
    MMTrapezoidEnvState_RELEASE
} MMTrapezoidEnvState;

typdef struct __MMTrapezoidEnv {
    MMLineFunc2D           lf;
    MMSample              min;
    MMSample              max;
    MMSample             time;
    MMSample       attackTime;
    MMSample      releaseTime;
    MMTrapezoidEnvState state;
} MMTrapezoidEnv;

MMTrapezoidEnv_reset(MMTrapezoidEnv *te);
void MMTrapezoidEnv_init(MMTrapezoidEnv *te,
                         MMSample min,
                         MMSample max,
                         MMsample attackTime,
                         MMsample releaseTime);
MMSample MMTrapezoidEnv_getCurValue(MMTrapezoidEnv *te);
void MMTrapezoidEnv_incTime(MMTrapezoidEnv *te, MMSample deltaTime);
void MMTrapezoidEnv_startAttack(MMTrapezoidEnv *te);
void MMTrapezoidEnv_startRelease(MMTrapezoidEnv *te);

#endif /* MM_TRAPEZOID_ENV_H */
