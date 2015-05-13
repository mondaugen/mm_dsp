#include "mm_bussplitter.h" 
#include <string.h> 

static void MMBusSplitter_tick(MMSigProc *bm)
{
    MMSigProc_defaultTick(bm);
    size_t i, j;
    for (i = 0;
         i < ((MMBusSplitter*)bm)->sourceBus->size
            * ((MMBusSplitter*)bm)->sourceBus->channels;
         i += ((MMBusSplitter*)bm)->sourceBus->channels) {
        for (j = 0; j < ((MMBusSplitter*)bm)->sourceBus->channels; j++) {
            ((MMBusSplitter*)bm)->destBus->data[i + j] 
                    = ((MMBusSplitter*)bm)->sourceBus->data[i + j];
        }
    }
}

void MMBusSplitter_init(MMBusSplitter *bm, MMBus *sourceBus, MMBus *destBus)
{
    MMSigProc_init((MMSigProc*)bm);
    bm->sourceBus = sourceBus;
    bm->destBus = destBus;
    MMSigProc_setTick(bm,MMBusSplitter_tick);
}

