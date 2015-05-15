#include "mm_busmerger.h" 
#include <string.h> 

static void MMBusMerger_tick(MMSigProc *sp)
{
    MMSigProc_defaultTick(sp);
    MMBusMerger *bm = (MMBusMerger*)sp;
    size_t i, j, channels;
    channels = bm->sourceBus->channels <= bm->destBus->channels ?
        bm->sourceBus->channels : bm->destBus->channels;
    for (i = 0; i < bm->sourceBus->size; i++) {
        for (j = 0; j < channels; j++) {
            bm->destBus->data[i*bm->destBus->channels + j] +=
                bm->sourceBus->data[i*bm->sourceBus->channels + j];
        }
    }
}

/* Sums the contents of sourceBus into the destBus.
 * If the sourceBus has a smaller number of channels than the destBus the n
 * channels of the sourceBus are written to the first n channels of the destBus.
 * If the sourceBus has a greater number of channels than the destBus then the
 * first m channels of the sourceBus are written to the channels of the destBus.
 *
 * Both the sourceBus and the destBus must have the same size. 
 *
 */
void MMBusMerger_init(MMBusMerger *bm, MMBus *sourceBus, MMBus *destBus)
{
    MMSigProc_init((MMSigProc*)bm);
    bm->sourceBus = sourceBus;
    bm->destBus = destBus;
    MMSigProc_setTick(bm,MMBusMerger_tick);
}
    
