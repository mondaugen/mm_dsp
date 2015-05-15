#include "mm_bussplitter.h" 
#include <string.h> 

static void MMBusSplitter_tick(MMSigProc *sp)
{
    MMSigProc_defaultTick(sp);
    MMBusSplitter *bm = (MMBusSplitter*)sp;
    size_t i, j, channels;
    channels = bm->sourceBus->channels <= bm->destBus->channels ?
        bm->sourceBus->channels : bm->destBus->channels;
    for (i = 0; i < bm->sourceBus->size; i++) {
        for (j = 0; j < channels; j++) {
            bm->destBus->data[i*bm->destBus->channels + j] =
                bm->sourceBus->data[i*bm->sourceBus->channels + j];
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

