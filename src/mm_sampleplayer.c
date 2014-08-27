#include "mm_sampleplayer.h" 
#include <string.h> 

/* Writes a sample according to lookup speed and wavetable into the busses of
 * the sample player. No interpolation for now. */
MMSigProc_Err MMSamplePlayerSigProc_tick(SigProc *sp)
{
   size_t i;
   MMSamplePlayerSigProc *spsp = (SigProc*)sp;
   /* we could have a switch for interpolation here... */
   *(spsp->parent->bus) += MMWavTab_get(spsp->samples,spsp->index);
   spsp->index += rate;
   if (spsp->loop) {
       while (spsp->index >= spsp->samples.length) {
           spsp->index -= spsp->samples.length;
       }
       while (spsp->index < 0) {
           spsp->index += spsp->samples.length;
       }
   } else if ((spsp->index >= spsp->samples.length)
           || (spsp->index < 0)) {
       spsp->state = MMSigProc_State_DONE;
   }
   /* we handle the doneAction */
   if ((spsp->state == MMSigProc_State_DONE) 
           && (spsp->doneAction == MMSigProc_DoneAction_FREE)) {
       spsp = (MMSamplePlayerSigProc*)MMDLList_remove((MMDLList*)spsp);
       MMSamplePlayerSigProc_free(spsp); 
   }
   return MMSigProc_Err_GOOD;
}

MMSamplePlayerSigProc *MMSamplePlayerSigProc_new(void)
{
    return (MMSamplePlayerSigProc*)malloc(sizeof(MMSamplePlayerSigProc));
}

void MMSamplePlayerSigProc_init(MMSamplePlayerSigProc *spsp)
{
    memset(spsp,0,sizeof(MMSamplePlayerSigProc));
    MMSigProc_init((MMSigProc*)spsp);
    spsp->tick = MMSamplePlayerSigProc_tick;
}

MMSamplePlayer *MMSamplePlayer_new(void)
{
    return (MMSamplePlayer*)malloc(sizeof(MMSamplePlayer));
}

void MMSamplePlayer_init(MMSamplePlayer *sp)
{
    memset(sp,0,sizeof(MMSamplePlayer));
    MMSigProc_init(sp->placeholder);
}
