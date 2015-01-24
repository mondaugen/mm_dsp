#include "mm_busmult.h"

static MMSigProc_Err MMBusMult_tick(MMSigProc *bm)
{
    /* Call superclass tick method */
    MMSigProc_Err result;
    if ((result = MMSigProc_defaultTick(bm)) != MMSigProc_Err_GOOD) {
        return result;
    }
    size_t i, j;
    for (i = 0; i < ((MMBusMult*)bm)->resultBus->size;
            i += ((MMBusMult*)bm)->resultBus->channels) {
        for (j = 0; j < ((MMBusMult*)bm)->resultBus->channels; j++) {
            ((MMBusMult*)bm)->resultBus->data[i + j] 
                    *= ((MMBusMult*)bm)->otherBus->data[i + j];
        }
    }
    return result;
}
