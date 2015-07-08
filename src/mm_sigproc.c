#include <stdlib.h> 
#include <string.h> 
#include "mm_sigproc.h" 
#ifdef MM_DSP_DEBUG
 #include <assert.h>
#endif  

void MMSigProc_defaultTick(MMSigProc *sp)
{
    return;
}

void MMSigProc_free(void *sp)
{
    free(sp);
}

void MMSigProc_init(MMSigProc *sp)
{
    memset(sp,0,sizeof(MMSigProc));
    MMDLList_init(sp);
    sp->tick = MMSigProc_defaultTick;
    sp->state = MMSigProc_State_UNKNOWN;
    sp->doneAction = MMSigProc_DoneAction_NONE;
}

MMSigProc *MMSigProc_new(void)
{
    MMSigProc * result;
    result = (MMSigProc*)malloc(sizeof(MMSigProc));
#ifdef MM_DSP_DEBUG
    assert(result);
#endif  
    return result;
}
