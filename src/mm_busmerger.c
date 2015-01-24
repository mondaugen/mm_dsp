#include "mm_busmerger.h" 
#include <string.h> 

static void MMBusMerger_tick(MMSigProc *bm)
{
    MMSigProc_defaultTick(bm);
    size_t i, j;
    for (i = 0; i < ((MMBusMerger*)bm)->sourceBus->size;
            i += ((MMBusMerger*)bm)->sourceBus->channels) {
        for (j = 0; j < ((MMBusMerger*)bm)->sourceBus->channels; j++) {
            ((MMBusMerger*)bm)->destBus->data[i + j] 
                    += ((MMBusMerger*)bm)->sourceBus->data[i + j];
        }
    }
}

void MMBusMerger_init(MMBusMerger *bm, MMBus *sourceBus, MMBus *destBus)
{
    MMSigProc_init((MMSigProc*)bm);
    bm->sourceBus = sourceBus;
    bm->destBus = destBus;
    MMSigProc_setTick(bm,MMBusMerger_tick);
}
    
