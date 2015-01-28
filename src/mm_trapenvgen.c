/* Only writes values to first channel in the bus */
void MMTrapEnvGen_tick(MMSigProc *teg)
{
    size_t i, j;
    for (   i = 0;
            i < MMEnvGen_getBus(teg).size;
            i += MMEnvGen_getBus(teg).channels) {
        MMEnvGen_getBus->data[i] = MMTrapezoidEnv_getCurValue_FAST(&teg->te);
        MMTrapezoidEnv_incTime_FAST(&teg->te,MMEnvGen_getDeltaTime(teg));
    }
}

void MMTrapEnvGen_init(  MMTrapEnvGen *teg,
                         MMBus    outBus,
                         MMSample deltaTime,
                         MMSample min,
                         MMSample max,
                         MMSample attackTime,
                         MMSample releaseTime)
{
    MMTrapEnvGen_init(teg, outBus, deltaTime);
    MMTrapezoidEnv_init(&teg->te,min,max,attackTime,releaseTime);
    MMSigProc_setTick(teg,MMTrapEnvGen_tick);
}
