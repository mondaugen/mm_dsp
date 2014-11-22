#include "mm_busmerger.h" 
#include <string.h> 

static MMSigProc_Err MMBusMerger_tick(MMSigProc *bm)
{
    /* Call superclass tick method */
    MMSigProc_Err result;
    if ((result = MMSigProc_defaultTick(bm)) != MMSigProc_Err_GOOD) {
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
    for (i = 0; i < ((MMBusMerger*)bm)->sourceBus->size;
            i += ((MMBusMerger*)bm)->sourceBus->channels) {
        for (j = 0; j < ((MMBusMerger*)bm)->sourceBus->channels; j++) {
            ((MMBusMerger*)bm)->destBus->data[i + j] 
                    += ((MMBusMerger*)bm)->sourceBus->data[i + j];
        }
    }
    return result;
}

void MMBusMerger_init(MMBusMerger *bm, MMBus *sourceBus, MMBus *destBus)
{
    MMSigProc_init((MMSigProc*)bm);
    bm->sourceBus = sourceBus;
    bm->destBus = destBus;
    MMSigProc_setTick(bm,MMBusMerger_tick);
}
    
