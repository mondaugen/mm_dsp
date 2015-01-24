#include <stdlib.h> 
#include <string.h> 
#include "mm_sigproc.h" 

void MMSigProc_free(void *sp)
{
    free(sp);
}

void MMSigProc_init(MMSigProc *sp)
{
    memset(sp,0,sizeof(MMSigProc));
    MMDLList_init(sp);
    sp->tick = NULL;
    sp->state = MMSigProc_State_UNKNOWN;
    sp->doneAction = MMSigProc_DoneAction_NONE;
}

MMSigProc *MMSigProc_new(void)
{
    return (MMSigProc*)malloc(sizeof(MMSigProc));
}
