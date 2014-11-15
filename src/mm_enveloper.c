#include "mm_enveloper.h" 

static MMSigProc_Err MMEnveloper_tick(MMSigProc *enver)
{
    /* Call superclass tick method */
    MMSigProc_Err result;
    if ((result = MMSigProc_defaultTick(enver)) != MMSigProc_Err_GOOD) {
        return result;
    }
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
    return result;
}

void MMEnveloper_init(MMEnveloper *enver, MMEnvelope *env, MMBus *outBus, MMSample tickPeriod)
{
    MMSigProc_init((MMSigProc*)enver);
    MMSigProc_setTick(enver,MMEnveloper_tick);
    enver->env = env;
    enver->outBus = outBus;
    enver->tickPeriod = tickPeriod;
}
