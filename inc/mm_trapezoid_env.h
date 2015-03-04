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
    MMSample      sustainTime; /* when >= 0, envelope triggers its own release,
                                  set to < 0 if you want to specify the release */
} MMTrapezoidEnv;

void MMTrapezoidEnv_init(MMTrapezoidEnv *te,
                         MMSample min,
                         MMSample max,
                         MMSample attackTime,
                         MMSample releaseTime,
                         MMSample sustainTime);

void MMTrapezoidEnv_startRelease(MMEnvelope *te);

#define MMTrapezoidEnv_setEnvParams(te,_min,_max,_attackTime,_releaseTime,_sustainTime)\
    MMTrapezoidEnv_get(te)->min = _min;\
    MMTrapezoidEnv_get(te)->max = _max;\
    MMTrapezoidEnv_get(te)->attackTime = _attackTime;\
    MMTrapezoidEnv_get(te)->releaseTime = _releaseTime;\
    MMTrapezoidEnv_get(te)->sustainTime = _sustainTime;

#define MMTrapezoidEnv_get(te) ((MMTrapezoidEnv *)te) 

#define MMTrapezoidEnv_getCurValue_FAST(te) \
    MMLineFunc2D_eval_FAST(MMTrapezoidEnv_get(te)->lf,MMEnvelope_get_time(te))

/* Increment the envelope's time by deltaTime and update internal state if time
 * has moved into a different region (attack -> sustain, release -> off) */
#define MMTrapezoidEnv_incTime_FAST(te,deltaTime) \
    do {\
        if (MMEnvelope_getState(te) == MMEnvelopeState_OFF) {\
            break;\
        }\
        if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_SUSTAIN)\
                && (((MMTrapezoidEnv*)te)->sustainTime < 0)) {\
            break;\
        }\
        MMEnvelope_get_time(te) += deltaTime;\
        if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_ATTACK)\
                && (MMEnvelope_get_time(te) >= MMTrapezoidEnv_get(te)->attackTime)) {\
            MMLineFunc2D_set_mb(&MMTrapezoidEnv_get(te)->lf, 0, MMTrapezoidEnv_get(te)->max);\
            MMEnvelope_getState(te) = MMTrapezoidEnvState_SUSTAIN;\
            MMEnvelope_get_time(te) = 0;\
        } else if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_SUSTAIN)\
                && (MMEnvelope_get_time(te) >= MMTrapezoidEnv_get(te)->sustainTime)) {\
            MMTrapezoidEnv_startRelease(te);\
        } else if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_RELEASE)\
                && (MMEnvelope_get_time(te) >= MMTrapezoidEnv_get(te)->releaseTime)) {\
            MMLineFunc2D_set_mb(&MMTrapezoidEnv_get(te)->lf, 0, MMTrapezoidEnv_get(te)->min);\
            MMEnvelope_getState(te) = MMEnvelopeState_OFF;\
        }\
    } while (0)

#endif /* MM_TRAPEZOID_ENV_H */
