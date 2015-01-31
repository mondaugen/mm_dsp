#ifndef MM_BUSMULT_H
#define MM_BUSMULT_H 

#include "mm_sigproc.h"
#include "mm_bus.h"

/* Multiples the second bus by the first bus, putting the result into ce
 * dernier */

typedef struct __MMBusMult MMBusMult;

struct __MMBusMult {
    MMSigProc head;
    MMBus *resultBus; /* The bus the result goes into */
    MMBus *otherBus;  /* The other argument in the multiplication */
};

void MMBusMult_init(MMBusMult *bm, MMBus *rb, MMBus *ob);

#endif /* MM_BUSMULT_H */
