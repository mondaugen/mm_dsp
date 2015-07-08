#include "mm_sampleplayer.h" 
#include "mm_stdlib.h" 
#include <string.h> 

#ifdef MM_DSP_DEBUG
 #include <assert.h>
#endif /* MM_DSP_DEBUG */ 

/* Accepts a MMSamplePlayerSigProc* casted to MMSigProc*.  If sp->outBus and
 * sp->samples aren't NULL, the outBus (which should only contain one channel,
 * see the init function) is filled with values interpolated from spsp->samples.
 * spsp->index is updated using spsp->rate.  This function assumes spsp->rate
 * and spsp->index are in Q_24_8 format.  If sp->outBus or sp->samples are NULL,
 * the function returns without writing anything to the bus. This means whatever
 * was in the bus before is still there. This is for speed: most likely this
 * will be used in conjunction with an envelope, which should be multiplying the
 * bus by 0 when spsp is not playing. To write zeros would be doing the same
 * operation twice.  Another consequence is that the length of playback is
 * quantized to the size of outBus, because the values of spsp can only be
 * updated between calls to this function (unless you change the values during 
 * interrupts, god forbid. This would result in a dereferencing of a NULL
 * pointer. Don't do this.). You see, another reason to use the envelope. */
static void MMSamplePlayerSigProc_tick_no_sum_interp_cubic(MMSigProc *sp)
{
    MMSigProc_defaultTick(sp);
    MMSamplePlayerSigProc *spsp = (MMSamplePlayerSigProc*)sp;
    if (spsp->outBus && spsp->samples) {
        MMWavTab_get_interpCubic_q_24_8_v(spsp->outBus->data,
                                          spsp->outBus->size,
                                          spsp->samples,
                                          &spsp->index,
                                          spsp->rate);
    }
}

static void MMSamplePlayerSigProc_tick_no_sum_interp_linear(MMSigProc *sp)
{
#ifdef MM_DSP_DEBUG 
    assert(0);
#endif /* MM_DSP_DEBUG */ 
}

static void MMSamplePlayerSigProc_tick_no_sum_interp_none(MMSigProc *sp)
{
#ifdef MM_DSP_DEBUG 
    assert(0);
#endif /* MM_DSP_DEBUG */ 
}

MMSamplePlayerSigProc *MMSamplePlayerSigProc_new(void)
{
    MMSamplePlayerSigProc * result = 
        (MMSamplePlayerSigProc*)malloc(sizeof(MMSamplePlayerSigProc));
#ifdef MM_DSP_DEBUG
    assert(result);
#endif  
    return result;
}

/* Pass a filled in MMSamplePlayerSigProcInitStuct into this function. If you
 * don't know what to initialize with, put NULL or 0. */
void MMSamplePlayerSigProc_init(MMSamplePlayerSigProc *sp,
        MMSamplePlayerSigProcInitStruct *init)
{
    memset(sp,0,sizeof(MMSamplePlayerSigProc));
    MMSigProc_init((MMSigProc*)sp);
    sp->interp = init->interp;
    if (init->tickType == MMSamplePlayerTickType_NOSUM) {
        switch (sp->interp) {
            case MMInterpMethod_NONE:
                MMSigProc_setTick(sp,
                        MMSamplePlayerSigProc_tick_no_sum_interp_none);
                break;
            case MMInterpMethod_LINEAR:
                MMSigProc_setTick(sp,
                        MMSamplePlayerSigProc_tick_no_sum_interp_linear);
                break;
            case MMInterpMethod_CUBIC:
                MMSigProc_setTick(sp,
                        MMSamplePlayerSigProc_tick_no_sum_interp_cubic);
                break;
            default:
                MMSigProc_setTick(sp,
                        MMSamplePlayerSigProc_tick_no_sum_interp_linear);
                break;
        }
    } else {
#ifdef MM_DSP_DEBUG
        assert(0); /* The sum version is not available. */
#endif /* MM_DSP_DEBUG */ 
    }
    sp->outBus = init->outBus;
#ifdef MM_DSP_DEBUG
    assert(sp->outBus->channels == 1); 
#endif /* MM_DSP_DEBUG */ 
    sp->samples = init->samples;
    sp->index = 0;
    sp->rate = 0;
    sp->note = 0;
}
