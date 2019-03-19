#ifndef MM_ENVEDSAMPLEPLAYER_TWOBUS_H
#define MM_ENVEDSAMPLEPLAYER_TWOBUS_H 

#include "mm_envedsampleplayer.h"
#include "mm_busmerger.h" 

typedef struct {
    MMEnvedSamplePlayer *esp;
} MMEnvedSamplePlayerTwoBusInitStruct;

typedef struct {
    MMEnvedSamplePlayer *esp;
    /* Merges spsp's outBus with outBus2 */
    MMBusMerger bus_merger;
} MMEnvedSamplePlayerTwoBus;

void MMEnvedSamplePlayerTwoBus_init(MMEnvedSamplePlayerTwoBus *esp,
                              MMEnvedSamplePlayerTwoBusInitStruct *init);

void MMEnvedSamplePlayerTwoBus_set_out_bus(MMEnvedSamplePlayerTwoBus *esp, MMBus *outBus);

#endif /* MM_ENVEDSAMPLEPLAYER_TWOBUS_H */
