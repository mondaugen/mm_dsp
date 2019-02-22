/* Do aribitrary processing on a bus. */

#include <stdlib.h>
#include "mm_sigproc.h"
#include "mm_bus.h"
#include "mm_busproc.h"

struct _MMBusProc {
    MMSigProc head;
    MMBus *bus;
    void (*fun)(MMBus *bus, void *aux);
    void *aux;
};

static void MMBusProc_tick(MMSigProc *bm_)
{
    MMBusProc *bp = (MMBusProc*)bm_;
    MMSigProc_defaultTick(bm_);
    bp->fun(bp->bus,bp->aux);
}

MMBusProc *
MMBusProc_new(
MMBus *bus,
void (*fun)(MMBus *bus, void *aux),
void *aux)
{
    MMBusProc *ret = calloc(1,sizeof(MMBusProc));
    if (!ret) { goto fail; }
    MMSigProc_init((MMSigProc*)ret);
    MMSigProc_setTick((MMSigProc*)ret,MMBusProc_tick);
    ret->bus = bus;
    ret->fun = fun;
    ret->aux = aux;
    return ret;
fail:
    return NULL;
}
