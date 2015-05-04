#include "mm_trapezoid_env.h" 

/* Resets to initial OFF state, where request for value give "min" */
static void MMTrapezoidEnv_reset(MMEnvelope *te)
{
    MMTrapezoidEnv_get(te)->lf.m = 0;
    MMTrapezoidEnv_get(te)->lf.b = MMTrapezoidEnv_get(te)->min;
    MMEnvelope_set_time(te,0);
    MMEnvelope_setState(te, MMEnvelopeState_OFF);
}

static MMSample MMTrapezoidEnv_getCurValue(MMEnvelope *te)
{
    return (MMSample)MMRationalFunc_eval(&MMTrapezoidEnv_get(te)->lf,MMEnvelope_get_time(te));
}

static void MMTrapezoidEnv_startAttack(MMEnvelope *te)
{
    MMSample curVal = MMTrapezoidEnv_getCurValue(te);
    MMLineFunc2D_set_points(&MMTrapezoidEnv_get(te)->lf, 0, curVal, 
            MMTrapezoidEnv_get(te)->attackTime, MMTrapezoidEnv_get(te)->max);
    MMEnvelope_getState(te) = MMTrapezoidEnvState_ATTACK;
    MMEnvelope_get_time(te) = 0;
}

void MMTrapezoidEnv_startRelease(MMEnvelope *te)
{
    MMSample curVal = MMTrapezoidEnv_getCurValue(te);
    MMLineFunc2D_set_points(&MMTrapezoidEnv_get(te)->lf, 0, curVal, 
            MMTrapezoidEnv_get(te)->releaseTime, MMTrapezoidEnv_get(te)->min);
    MMEnvelope_getState(te) = MMTrapezoidEnvState_RELEASE;
    MMEnvelope_get_time(te) = 0;
}

/* Increment the envelope's time by deltaTime and update internal state if time
 * has moved into a different region (attack -> sustain, release -> off) */
static void MMTrapezoidEnv_incTime(MMEnvelope *te, MMSample deltaTime)
{
    if (MMEnvelope_getState(te) == MMEnvelopeState_OFF) {
        return;
    }
    if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_SUSTAIN)
            && (((MMTrapezoidEnv*)te)->sustainTime < 0)) {
        /* this means it will not keep track of how long the sustain region
         * has been going on and must wait for startRelease to be called */
        return;
    }
    MMEnvelope_get_time(te) += deltaTime;
    if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_ATTACK)
            && (MMEnvelope_get_time(te) >= MMTrapezoidEnv_get(te)->attackTime)) {
        /* Attack is finished, go to sustain region */
        MMLineFunc2D_set_mb(&MMTrapezoidEnv_get(te)->lf, 0, MMTrapezoidEnv_get(te)->max);
        MMEnvelope_getState(te) = MMTrapezoidEnvState_SUSTAIN;
        MMEnvelope_get_time(te) = 0;
    } else if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_SUSTAIN)
            && (MMEnvelope_get_time(te) >= MMTrapezoidEnv_get(te)->sustainTime)) {
        MMTrapezoidEnv_startRelease(te);
    } else if ((MMEnvelope_getState(te) == MMTrapezoidEnvState_RELEASE)
            && (MMEnvelope_get_time(te) >= MMTrapezoidEnv_get(te)->releaseTime)) {
        /* Release is finished, go to off region */
        MMLineFunc2D_set_mb(&MMTrapezoidEnv_get(te)->lf, 0, MMTrapezoidEnv_get(te)->min);
        MMEnvelope_getState(te) = MMEnvelopeState_OFF;
    }
}

/* releaseTime must be known upon init so that the envelope knows how long to
 * fade out for if stolen. If sustain time is >= 0. then the note will turn
 * itself off by keeping track of how long it has sustained for and initializing
 * the release section after this amount of time has passed. If sustain time <
 * 0, then the envelope expects that it will be turned off at a later point
 * (e.g., due to a note off message). */
void MMTrapezoidEnv_init(MMTrapezoidEnv *te,
                         MMSample min,
                         MMSample max,
                         MMSample attackTime,
                         MMSample releaseTime,
                         MMSample sustainTime)
{
    MMEnvelope_init((MMEnvelope *)te);
    MMEnvelope_set_getCurValue(te,MMTrapezoidEnv_getCurValue);
    MMEnvelope_set_incTime(te,MMTrapezoidEnv_incTime);
    MMEnvelope_set_startAttack(te,MMTrapezoidEnv_startAttack);
    MMEnvelope_set_startRelease(te,MMTrapezoidEnv_startRelease);
    MMEnvelope_set_reset(te,MMTrapezoidEnv_reset);
    MMLineFunc2D_init(&MMTrapezoidEnv_get(te)->lf);
    MMTrapezoidEnv_setEnvParams(MMTrapezoidEnv_get(te),min,max,attackTime,releaseTime,sustainTime);
    MMEnvelope_reset(te);
}
