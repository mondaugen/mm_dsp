#include "mm_busmerger.h" 
#include <string.h> 

static MMSigProc_Err MMBusMerger_tick(MMSigProc *bm)
{
    /* Call superclass tick method */
    MMSigProc_Err result;
    if ((result = MMSigProc_defaultTick(enver)) != MMSigProc_Err_GOOD) {
        return result;
    }
#ifdef MM_DSP_DEBUG 
    if ((bm->sourceBus.channels != bm->destBus.channels) 
            || (bm->sourceBus.size != bm->destBus.size)) {
        return MMSigProc_Err_NOTINIT; /* number of channels and size of source
                                         and destination busses need to be equal
                                         */
    }
#endif /* MM_DSP_DEBUG */ 
    size_t i, j;
    for (i = 0; i < bm->sourceBus->size; i += bm->sourceBus->channels) {
        for (j = 0; j < bm->sourceBus->channels; j++) {
            bm->destBus->data[i + j] += bm->sourceBus->data[i + j];
        }
    }
    return result;
}
