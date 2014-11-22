#ifndef MM_TRAPEZOID_ENV_H
#define MM_TRAPEZOID_ENV_H 
#include "mm_envelope.h" 
#include "mm_rationalfunc.h"

typedef enum {
    MMTrapezoidEnvState_OFF,
    MMTrapezoidEnvState_ATTACK,
    MMTrapezoidEnvState_SUSTAIN,
    MMTrapezoidEnvState_RELEASE,
} MMTrapezoidEnvState;

typedef struct __MMTrapezoidEnv {
    MMEnvelope           head; /* superclass */
    MMLineFunc2D           lf;
    MMSample              min;
    MMSample              max;
    MMSample       attackTime;
    MMSample      releaseTime;
    MMTrapezoidEnvState state;
} MMTrapezoidEnv;

void MMTrapezoidEnv_init(MMTrapezoidEnv *te,
                         MMSample min,
                         MMSample max,
                         MMSample attackTime,
                         MMSample releaseTime);

#define MMTrapezoidEnv_get(te) ((MMTrapezoidEnv *)te) 

#endif /* MM_TRAPEZOID_ENV_H */
