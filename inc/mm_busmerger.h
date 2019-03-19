#ifndef MM_BUSMERGER_H
#define MM_BUSMERGER_H 

#include "mm_sigproc.h"
#include "mm_bus.h" 

/* Allows one bus to write into another bus, mixing with contents of the
 * destination bus */

typedef struct __MMBusMerger MMBusMerger;

struct __MMBusMerger {
    MMSigProc head;
    MMBus *sourceBus;
    MMBus *destBus;
};

void MMBusMerger_init(MMBusMerger *bm, MMBus *sourceBus, MMBus *destBus);

void MMBusMerger_set_destBus(MMBusMerger *bm, MMBus *destBus);

#endif /* MM_BUSMERGER_H */
