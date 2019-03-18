#include "mm_bus.h" 
#include "mm_busmerger.h" 
#include "mm_bussplitter.h"
#include "mm_sigproc.h" 

typedef struct {
    MMEnvedSamplePlayerInitStruct super;
    MMBus *outBus2;
} MMEnvedSamplePlayerTwoBusInitStruct;

typedef struct {
    MMEnvedSamplePlayer super;
    /* Bus where output is written and stored briefly before merged with outBus2 */
    MMBus *tmpBus;
    /* Splits the main output off into tmpBus */
    MMBusSplitter bus_splitter;
    /* Merges tmpBus with outBus2 */
    MMBusMerger bus_merger;
} MMEnvedSamplePlayerTwoBus;

void MMEnvedSamplePlayerTwoBus_init(MMEnvedSamplePlayerTwoBus *esp,
                              MMEnvedSamplePlayerTwoBusInitStruct *init)
{
    MMEnvedSamplePlayer_init((MMEnvedSamplePlayer *)esp,
                              (MMEnvedSamplePlayerInitStruct *)init);
    /* Simply just add the tmpBus, bus_splitter and bus_merger */
    esp->tmpBus = MMBus_new(esp->outBus2->size,esp->outBus2->channels);
    MMBusSplitter_init(&esp->bus_splitter,esp->super.spsp.outBus,esp->tmpBus);
    /* TODO: You need to get the end of the internal signal chain in order to
    sum this in to the correct spot */
    // MMSigProc_insertAfter(esp->super, &esp->busMult);

    MMBusMerger_init(&esp->bus_merger,esp->tmpBus,init->outBus2)
}
