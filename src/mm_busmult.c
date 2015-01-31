#include "mm_busmult.h"

static void MMBusMult_tick(MMSigProc *bm)
{
    MMSigProc_defaultTick(bm);
    size_t i, j;
    for (i = 0; i < ((MMBusMult*)bm)->resultBus->size;
            i += ((MMBusMult*)bm)->resultBus->channels) {
        for (j = 0; j < ((MMBusMult*)bm)->resultBus->channels; j++) {
            ((MMBusMult*)bm)->resultBus->data[i + j] 
                    *= ((MMBusMult*)bm)->otherBus->data[i + j];
        }
    }
}

void MMBusMult_init(MMBusMult *bm, MMBus *rb, MMBus *ob)
{
    MMSigProc_init((MMSigProc*)bm);
    MMSigProc_setTick(bm,MMBusMult_tick);
    (bm)->resultBus = rb;
    (bm)->otherBus  = ob;
}
