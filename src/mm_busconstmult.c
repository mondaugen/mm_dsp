/* mm_busconstmult.c 
 *
 * Multiply a bus by a constant which can be updated between ticks.
 * The constant is linearly interpolated to its next value for smooth
 * transitions.
 */

#include "mm_busconstmult.h" 

static void
MMBusConstMult_tick(MMSigProc *sp)
{
    MMBusConstMult *bcm = (MMBusConstMult*)sp;
    MMSigProc_defaultTick(sp);
    MMSample cinc;
    if (bcm->newConst) {
        cinc = (*bcm->newConst - bcm->lastConst)
            / bcm->bus->size;
    } else {
        cinc = 0.; /* Will just keep multiplying by bcm->lastConst */
    }
    size_t i, j;
    for (i = 0;
            i < (bcm->bus->size * bcm->bus->channels);
            i += bcm->bus->channels) {
        for (j = 0;
                j < bcm->bus->channels;
                j++) {
            bcm->bus->data[i+j] *=
                bcm->lastConst;
        }
        bcm->lastConst += cinc;
    }
}

void MMBusConstMult_init(MMBusConstMult *bcm,
                         MMBus *bus,
                         MMSample lastConst)
{
    MMSigProc_init((MMSigProc*)bcm);
    MMSigProc_setTick(bcm,MMBusConstMult_tick);
    bcm->bus = bus;
    bcm->lastConst = lastConst;
    bcm->newConst = NULL;
}

