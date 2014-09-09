#include "mm_sigchain.h"
#include <stddef.h> 

static MMSigProc_Err MMSigChain_tick(MMSigProc *sp)
{
    MMSigProc *current, *next;
    current = &((MMSigChain*)sp)->sigProcs;
    do {
        /* we store the next item in the list because MMSigProc_tick could free current making its next NULL */
        next = (MMSigProc*)MMDLList_getNext(current); 
        MMSigProc_tick(current);
        current = next;
    } while (current);
    return MMSigProc_Err_GOOD;
}

void MMSigChain_init(MMSigChain *sc)
{
    MMSigProc_init((MMSigProc*)sc);
    MMSigProc_init(&(sc->sigProcs));
    ((MMSigProc*)sc)->tick = MMSigChain_tick; /* put in new tick function for this sub class */
}

void MMSigChain_markToFree(MMSigChain *sc, MMSigProc *sp MMSigProc_FreeFunc *freeFunc)
{
    

