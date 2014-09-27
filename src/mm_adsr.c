#include "mm_adsr.h"

static MMSigProc_Err MMAdsr_tick(MMSigProc *sp)
{
    /* Call superclass tick method */
    MMSigProc_Err result;
    if ((result = MMSigProc_defaultTick(sp)) != MMSigProc_Err_GOOD) {
        return result;
    }
    /* Constrain between 0 and 1 */
    if (((MMAdsr*)sp)->value < 0) {
        *(((MMAdsr*)sp)->bus) = 0; 
    } else if (((MMAdsr*)sp)->value < 1) {
        *(((MMAdsr*)sp)->bus) *= (((MMAdsr*)sp)->value);
    }
    if (((MMAdsr*)sp)->state == MMAdsr_State_DONE) {
        return MMSigProc_Err_IDLE;
    }
    if (((MMAdsr*)sp)->state == MMAdsr_State_ATTACK) {
        if (((MMAdsr*)sp)->attack > 0) {
            ((MMAdsr*)sp)->value += (1. - ((MMAdsr*)sp)->value)
                * ((MMAdsr*)sp)->samplePeriod / ((MMAdsr*)sp)->attack;
            ((MMAdsr*)sp)->attack -= ((MMAdsr*)sp)->samplePeriod;
        } else {
            ((MMAdsr*)sp)->value = 1; /* if attack is 0 */
            ((MMAdsr*)sp)->state = MMAdsr_State_DECAY;
        }
    }
    if (((MMAdsr*)sp)->state == MMAdsr_State_DECAY) {
        if (((MMAdsr*)sp)->decay > 0) {
            ((MMAdsr*)sp)->value += (((MMAdsr*)sp)->sustain - ((MMAdsr*)sp)->value)
                * ((MMAdsr*)sp)->samplePeriod / ((MMAdsr*)sp)->decay;
            ((MMAdsr*)sp)->decay -= ((MMAdsr*)sp)->samplePeriod;
        } else {
            ((MMAdsr*)sp)->value = ((MMAdsr*)sp)->sustain; /* if decay is 0 */
            ((MMAdsr*)sp)->state = MMAdsr_State_SUSTAIN;
        }
    }
    if (((MMAdsr*)sp)->state == MMAdsr_State_RELEASE) {
        if (((MMAdsr*)sp)->release > 0) {
            ((MMAdsr*)sp)->value += (-1. * ((MMAdsr*)sp)->value)
                * ((MMAdsr*)sp)->samplePeriod / ((MMAdsr*)sp)->release;
            ((MMAdsr*)sp)->release -= ((MMAdsr*)sp)->samplePeriod;
        } else {
            ((MMAdsr*)sp)->value = 0; /* if release is 0 */
            ((MMAdsr*)sp)->state = MMAdsr_State_DONE;
        }
    }
    return MMSigProc_Err_GOOD;
}

void MMAdsr_init(MMAdsr *adsr)
{
    MMSigProc_init((MMSigProc*)adsr);
    ((MMSigProc*)adsr)->tick = MMAdsr_tick; /* put in new tick function for this sub class */
    adsr->bus = NULL; /* user must set this so we don't segfault, it is slow to check every time */
    adsr->value = 0;
    adsr->state = MMAdsr_State_DONE;
    /* the rest is up to the user to set properly */
}

