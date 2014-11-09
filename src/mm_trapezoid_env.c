#include "mm_trapezoid_env.h" 

/* Resets to initial OFF state, where request for value give "min" */
MMTrapezoidEnv_reset(MMTrapezoidEnv *te)
{
    te->lf.m = 0;
    te->lf.b = te->min;
    te->time = 0;
    te->state = MMTrapezoidEnvState_OFF;
}

void MMTrapezoidEnv_init(MMTrapezoidEnv *te,
                         MMSample min,
                         MMSample max,
                         MMsample attackTime,
                         MMsample releaseTime)
{
    MMLineFunc2D_init(&te->lf);
    te->min = min;
    te->max = max;
    te->attackTime = attackTime;
    te->releaseTime = releaseTime;
    MMTrapezoidEnv_reset(te);
}

MMSample MMTrapezoidEnv_getCurValue(MMTrapezoidEnv *te)
{
    return (MMSample)MMRationalFunc_eval(&te->lf,te->time);
}

/* Increment the envelope's time by deltaTime and update internal state if time
 * has moved into a different region (attack -> sustain, release -> off) */
void MMTrapezoidEnv_incTime(MMTrapezoidEnv *te, MMSample deltaTime)
{
    if ((te->state == MMTrapezoidEnvState_OFF)
            || (te->state == MMTrapezoidEnvState_STATE)) {
        return;
    }
    te->time += deltaTime;
    if ((te->state == MMTrapezoidEnvState_ATTACK)
            && (te->time >= te->attackTime)) {
        /* Attack is finished, go to sustain region */
        MMLineFunc2D_set_mb(&te->lf, 0, te->max);
        te->state = MMTrapezoidEnvState_SUSTAIN;
    } else if ((te->state == MMTrapezoidEnvState_RELEASE)
            && (te->time >= te->releaseTime)) {
        /* Release is finished, go to off region */
        MMLineFunc2D_set_mb(&te->lf, 0, te->min);
        te->state = MMTrapezoidEnvState_OFF;
    }
}

void MMTrapezoidEnv_startAttack(MMTrapezoidEnv *te)
{
    MMSample curVal = MMTrapezoidEnv_getCurValue(te);
    MMLineFunc2D_set_points(&te->lf, 0, curVal, te->attackTime, te->max);
    te->state = MMTrapezoidEnvState_ATTACK;
    te->time = 0;
}

void MMTrapezoidEnv_startRelease(MMTrapezoidEnv *te)
{
    MMSample curVal = MMTrapezoidEnv_getCurValue(te);
    MMLineFunc2D_set_points(&te->lf, 0, curVal, te->releaseTime, te->min);
    te->state = MMTrapezoidEnvState_DECAY;
    te->time = 0;
}

#endif /* MM_TRAPEZOID_ENV_H */
