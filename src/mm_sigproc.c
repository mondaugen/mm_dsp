#include <stdlib.h> 
#include <string.h> 
#include "mm_sigproc.h" 

/* Global Malloc must have been initialized before running this stuff! */

void MMSigProc_free(void *sp)
{
    free(sp);
}

MMSigProc_Err MMSigProc_defaultTick(MMSigProc *sp)
{
    if (sp->state == MMSigProc_State_WAIT_FREE) {
        sp = (MMSigProc*)MMSigProc_remove(sp);
        MMSigProc_free(sp);
        return MMSigProc_Err_JUST_FREED;
    }
    return MMSigProc_Err_GOOD;
}

void MMSigProc_init(MMSigProc *sp)
{
    memset(sp,0,sizeof(MMSigProc));
    sp->tick = MMSigProc_defaultTick;
    sp->state = MMSigProc_State_UNKNOWN;
    sp->doneAction = MMSigProc_DoneAction_NONE;
}

MMSigProc *MMSigProc_new(void)
{
    return (MMSigProc*)malloc(sizeof(MMSigProc));
}
