#include "mm_trapenvgen.h"

/* Only writes values to first channel in the bus */
void MMTrapEnvGen_tick(MMSigProc *teg)
{
    size_t i;
    for (   i = 0;
            i < MMEnvGen_getBus(teg)->size;
            i += MMEnvGen_getBus(teg)->channels) {
        MMEnvGen_getBus(teg)->data[i] = 
            MMTrapezoidEnv_getCurValue_FAST(&((MMTrapEnvGen*)teg)->te);
        MMTrapezoidEnv_incTime_FAST(&((MMTrapEnvGen*)teg)->te,
                MMEnvGen_getDeltaTime(teg));
    }
}

void MMTrapEnvGen_init(  MMTrapEnvGen *teg,
                         MMBus    *outBus,
                         MMSample deltaTime,
                         MMSample min,
                         MMSample max,
                         MMSample attackTime,
                         MMSample releaseTime)
{
    MMEnvGen_init(teg, outBus, deltaTime);
    MMTrapezoidEnv_init(&teg->te,min,max,attackTime,releaseTime);
    MMSigProc_setTick(teg,MMTrapEnvGen_tick);
}
