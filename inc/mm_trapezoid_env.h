#ifndef MM_TRAPEZOID_ENV_H
#define MM_TRAPEZOID_ENV_H 
#include "mm_envelope.h" 
#include "mm_rationalfunc.h"

typedef enum {
    MMTrapezoidEnvState_ATTACK = MMEnvelopeState_DUMMY,
    MMTrapezoidEnvState_SUSTAIN,
    MMTrapezoidEnvState_RELEASE
} MMTrapezoidEnvState;

typedef struct __MMTrapezoidEnv {
    MMEnvelope           head; /* superclass */
    MMLineFunc2D           lf;
    MMSample              min;
    MMSample              max;
    MMSample       attackTime;
    MMSample      releaseTime;
} MMTrapezoidEnv;

void MMTrapezoidEnv_init(MMTrapezoidEnv *te,
                         MMSample min,
                         MMSample max,
                         MMSample attackTime,
                         MMSample releaseTime);

#define MMTrapezoidEnv_setEnvParams(_min,_max,_attackTime,_releaseTime)\
    MMTrapezoidEnv_get(te)->min = _min;\
    MMTrapezoidEnv_get(te)->max = _max;\
    MMTrapezoidEnv_get(te)->attackTime = _attackTime;\
    MMTrapezoidEnv_get(te)->releaseTime = _releaseTime;

#define MMTrapezoidEnv_get(te) ((MMTrapezoidEnv *)te) 

#define MMTrapezoidEnv_getCurValue_FAST(te) \
    MMLineFunc2D_eval_FAST(MMTrapezoidEnv_get(te)->lf,MMEnvelope_get_time(te))

/* Increment the envelope's time by deltaTime and update internal state if time
 * has moved into a different region (attack -> sustain, release -> off) */
#define MMTrapezoidEnv_incTime_FAST(te,deltaTime) \
    do {\
        if ((MMEnvelope_getState(te) == MMEnvelopeState_OFF)\
            || (MMEnvelope_getState(te) == MMEnvelopeState_SUSTAIN)) {\
            break;\
        }\
        MMEnvelope_get_time(te) += deltaTime;\
        if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_ATTACK)\
            && (MMEnvelope_get_time(te) >= MMTrapezoidEnv_get(te)->attackTime)) {\
            /* Attack is finished, go to sustain region */\
            MMLineFunc2D_set_mb_FAST(MMTrapezoidEnv_get(te)->lf,\
                    0, MMTrapezoidEnv_get(te)->max);\
            MMEnvelope_getState(te) = MMTrapezoidEnvState_SUSTAIN;\
            break;\
        }\
        if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_RELEASE)\
            && (MMEnvelope_get_time(te) >= MMTrapezoidEnv_get(te)->releaseTime)) {\
            /* Release is finished, go to off region */\
            MMLineFunc2D_set_mb_FAST(MMTrapezoidEnv_get(te)->lf,\
                    0, MMTrapezoidEnv_get(te)->min);\
            MMEnvelope_getState(te) = MMEnvelopeState_OFF;\
            break;\
        }\
    } while (0)

#endif /* MM_TRAPEZOID_ENV_H */
