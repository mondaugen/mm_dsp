#ifndef MM_BUSSPLITTER_H
#define MM_BUSSPLITTER_H 

#include "mm_sigproc.h"
#include "mm_bus.h" 

/* Allows one bus to write into another bus, overwriting the contents of the
 * destination bus */

typedef struct __MMBusSplitter MMBusSplitter;

struct __MMBusSplitter {
    MMSigProc head;
    MMBus *sourceBus;
    MMBus *destBus;
};

void MMBusSplitter_init(MMBusSplitter *bm, MMBus *sourceBus, MMBus *destBus);
void MMBusSplitter_set_destBus(MMBusSplitter *bm, MMBus *destBus);

#endif /* MM_BUSSPLITTER_H */
