#include "mm_bus.h" 
#include "mm_sigproc.h" 
#include "mm_envedsampleplayer_twobus.h"

/* The MMEnvedSamplePlayer must be intialized before calling this */
void MMEnvedSamplePlayerTwoBus_init(MMEnvedSamplePlayerTwoBus *esp,
                              MMEnvedSamplePlayerTwoBusInitStruct *init)
{
    /* bus merger has no other bus, must be set */
    MMBusMerger_init(&esp->bus_merger,init->esp->spsp.outBus,NULL);
    MMSigProc *super_last_node = MMEnvedSamplePlayer_get_last_node(init->esp);
    MMSigProc_insertAfter(super_last_node,&esp->bus_merger);
}

void
MMEnvedSamplePlayerTwoBus_set_out_bus(MMEnvedSamplePlayerTwoBus *esp, MMBus *outBus)
{
    MMBusMerger_set_destBus(&esp->bus_merger,outBus);
}
