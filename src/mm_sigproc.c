#include <stdlib.h> 
#include <string.h> 
#include "mm_sigproc.h" 

MMSigProc_Err MMSigProc_defaultTick(MMSigProc *sp)
{
    /* do nothing */
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
