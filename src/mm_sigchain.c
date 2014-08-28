#include "mm_sigchain.h"
#include <stddef.h> 

static MMSigProc_Err MMSigChain_tick(MMSigProc *sp)
{
    MMSigProc *current;
    for (current = &((MMSigChain*)sp)->sigProcs;
            current != NULL;
            current = (MMSigProc*)MMDLList_getNext(current)) {
        MMSigProc_tick(current);
/*         MMSigProc_handleState(current); */
    }
    return MMSigProc_Err_GOOD;
}

void MMSigChain_init(MMSigChain *sc)
{
    MMSigProc_init((MMSigProc*)sc);
    MMSigProc_init(&(sc->sigProcs));
    ((MMSigProc*)sc)->tick = MMSigChain_tick; /* put in new tick function for this sub class */
}

