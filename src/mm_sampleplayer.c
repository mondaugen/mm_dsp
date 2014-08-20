#include "mm_sampleplayer.h" 

/* Writes a sample according to lookup speed and wavetable into the busses of
 * the sample player. No interpolation for now. */
MMSigProc_Err MMSamplePlayerSigProc_tick(SigProc *sp)
{
   size_t i; Sample tmp;
   MMSamplePlayerSigProc *spsp = (SigProc*)sp;
   for (i = 0; i < spsp->outBusses.length; i++) {
       MMBus bus = MMBusArray_get(spsp->parent->outBusses,i);
       /* we could have a switch for interpolation here... */
       MMWavTab_get(spsp->samples,spsp->index,&tmp);
       *bus += tmp;
   }
   /* we could handle the state or doneAction here */
   return MMSigProc_Err_GOOD;
}

