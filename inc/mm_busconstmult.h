#ifndef MM_BUSCONSTMULT_H
#define MM_BUSCONSTMULT_H 

#include "mm_sigproc.h"
#include "mm_sample.h"
#include "mm_bus.h"

typedef struct __MMBusConstMult MMBusConstMult;

struct __MMBusConstMult {
    MMSigProc head;
    MMBus *bus;
    /* The constant that will be interpolated from. */
    MMSample lastConst; 
    /* The constant that will be interpolated to. */
    MMSample newConst;
};

#endif /* MM_BUSCONSTMULT_H */
