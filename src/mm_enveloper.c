#include "mm_enveloper.h" 

static void MMEnveloper_tick(MMSigProc *enver)
{
    MMSigProc_defaultTick(enver);
    /* envelopes all channels */
    size_t i, j;
    for (   i = 0; 
            i < (MMEnveloper_get(enver)->outBus->size * MMEnveloper_get(enver)->outBus->channels);
            i += MMEnveloper_get(enver)->outBus->channels   ) {
        for (j = 0; j < MMEnveloper_get(enver)->outBus->channels; j++) {
            MMEnveloper_get(enver)->outBus->data[i + j] 
                *= MMEnvelope_getCurValue(MMEnveloper_get(enver)->env);
        }
        MMEnvelope_incTime(MMEnveloper_get(enver)->env,MMEnveloper_get(enver)->tickPeriod);
    }
}

void MMEnveloper_init(MMEnveloper *enver, MMEnvelope *env, MMBus *outBus, MMSample tickPeriod)
{
    MMSigProc_init((MMSigProc*)enver);
    MMSigProc_setTick(enver,MMEnveloper_tick);
    enver->env = env;
    enver->outBus = outBus;
    enver->tickPeriod = tickPeriod;
}
