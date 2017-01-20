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
 * and spsp->index are in Q_24_8 format. If spsp->samples are null the bus is
 * filled with 0s. It would be nice if instead the values in the bus were just
 * left as is and it was up to a following envelope to set values to 0, saving
 * some computation. A problem with this is that if the values in the bus are
 * NaN, multiplying by 0 will not yield 0. This is a more fool-proof way if for
 * some reason the bus contains NaNs.
 * Another consequence is that the length of playback is
 * quantized to the size of outBus, because the values of spsp can only be
 * updated between calls to this function (unless you change the values during 
 * interrupts, god forbid. This would result in a dereferencing of a NULL
 * pointer. Don't do this.). You see, another reason to use the envelope. */
static void MMSamplePlayerSigProc_tick_no_sum_interp_cubic(MMSigProc *sp)
{
    MMSigProc_defaultTick(sp);
    MMSamplePlayerSigProc *spsp = (MMSamplePlayerSigProc*)sp;
    if (spsp->samples) {
        if (spsp->p_rate) {
            /* TODO: Could be optimized by assuring outBus has size as power of
             * 2 */
            mm_q8_24_t rinc;
            rinc = (mm_q8_24_t)((*spsp->p_rate * (int64_t)spsp->rate) >> 24LL);
            rinc = (rinc - spsp->last_rate) / ((mm_q8_24_t)spsp->outBus->size);
            MMWavTab_get_interpCubic_rinc_q_8_24_idx_q_24_8_v(spsp->outBus->data,
                                                     spsp->outBus->size,
                                                     spsp->samples,
                                                     &spsp->index,
                                                     &spsp->last_rate,
                                                     rinc);
        } else {
            MMWavTab_get_interpCubic_q_24_8_v(spsp->outBus->data,
                                              spsp->outBus->size,
                                              spsp->samples,
                                              &spsp->index,
                                              spsp->rate);
        }
    } else {
        /* Assume the bus is only one channel wide. This is caught on
         * initialization of MMSamplePlayerSigProc if
         * DEBUG is enabled. */
        memset(spsp->outBus->data,0,spsp->outBus->size*sizeof(MMSample));
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
        assert(sp->outBus); /* outBus shouldn't be NULL */
#endif /* MM_DSP_DEBUG */ 
#ifdef MM_DSP_DEBUG
    assert(sp->outBus->channels == 1); 
#endif /* MM_DSP_DEBUG */ 
    sp->samples = init->samples;
    sp->index = 0;
    sp->rate = 0;
    sp->p_rate = NULL;
    sp->note = 0;
}
