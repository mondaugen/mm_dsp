#include "mm_sampleplayer.h" 
#include "mm_stdlib.h" 
#include <string.h> 

#ifdef MM_DSP_DEBUG
 #include <assert.h>
#endif /* MM_DSP_DEBUG */ 

/* Writes a sample according to lookup speed and wavetable into the busses of
 * the sample player. No interpolation for now. */
static void MMSamplePlayerSigProc_tick_sum(MMSigProc *sp)
{
    /* Call superclass tick method */
    MMSigProc_defaultTick(sp);
    if (MMSigProc_getState(sp) == MMSigProc_State_DONE) {
        return;
    }
    MMSamplePlayerSigProc *spsp = (MMSamplePlayerSigProc*)sp;
    size_t i;
    for (i = 0; 
         i < (spsp->parent->outBus->size * spsp->parent->outBus->channels);
         i += spsp->parent->outBus->channels) {
        /* The sample player is only compatible with busses of one channel, so
         * it will just write something to the first channel (0) and nothing to
         * the remaining channels */
        switch (spsp->interp) {
            case MMInterpMethod_NONE:
                MMSamplePlayerSigProc_getSampleInterpNone_sum_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            case MMInterpMethod_LINEAR:
                MMSamplePlayerSigProc_getSampleInterpLinear_sum_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            case MMInterpMethod_CUBIC:
                MMSamplePlayerSigProc_getSampleInterpCubicMsp_sum_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            default:
                *(spsp->parent->outBus->data + i) = 0;
                break;
        }
        spsp->index += spsp->rate;
        spsp->index = MM_wrapl(spsp->index, 0,
                ((MMSamplePlayerQ_t)MMArray_get_length(spsp->samples)) 
                << MMSAMPLEPLAYER_Q_WIDTH_FRAC);
    }
}

/* Writes a sample according to lookup speed and wavetable into the busses of
 * the sample player. */
/* THIS DOESN'T WORK */
static void MMSamplePlayerSigProc_tick_no_sum(MMSigProc *sp)
{
    /* Call superclass tick method */
    MMSigProc_defaultTick(sp);
    size_t i;
    MMSamplePlayerSigProc *spsp = (MMSamplePlayerSigProc*)sp;
    for (i = 0; 
         i < (spsp->parent->outBus->size * spsp->parent->outBus->channels);
         i += spsp->parent->outBus->channels) {
        /* The sample player is only compatible with busses of one channel, so
         * it will just write something to the first channel (0) and nothing to
         * the remaining channels */
        switch (spsp->interp) {
            case MMInterpMethod_NONE:
                MMSamplePlayerSigProc_getSampleInterpNone_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            case MMInterpMethod_LINEAR:
                MMSamplePlayerSigProc_getSampleInterpLinear_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            case MMInterpMethod_CUBIC:
                MMSamplePlayerSigProc_getSampleInterpCubicMsp_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            default:
                *(spsp->parent->outBus->data + i) = 0;
                break;
        }
        spsp->index += spsp->rate;
        if (spsp->loop) {
            spsp->index = MM_wrapl(spsp->index, 0,
                    ((MMSamplePlayerQ_t)MMArray_get_length(spsp->samples)) 
                        << MMSAMPLEPLAYER_Q_WIDTH_FRAC);
        } else if ((spsp->index 
                    >= (((MMSamplePlayerQ_t)MMArray_get_length(spsp->samples)) 
                        << MMSAMPLEPLAYER_Q_WIDTH_FRAC))
                || (spsp->index < 0)) {
            MMSigProc_setState(spsp,MMSigProc_State_DONE);
            /* Write 0 to the rest of the vector */
            while (i < (spsp->parent->outBus->size 
                        * spsp->parent->outBus->channels)) {
                *(spsp->parent->outBus->data + i) = 0;
                i += spsp->parent->outBus->channels;
            }
        }
    }
    /* we handle the doneAction */
    if ((MMSigProc_getState(spsp) == MMSigProc_State_DONE) 
            && (MMSigProc_getDoneAction(spsp) == MMSigProc_DoneAction_FREE)) {
        sp->state = MMSigProc_State_WAIT_FREE;
    }
}


MMSamplePlayerSigProc *MMSamplePlayerSigProc_new(void)
{
    return (MMSamplePlayerSigProc*)malloc(sizeof(MMSamplePlayerSigProc));
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

void MMSamplePlayerSigProc_setTickType(MMSamplePlayerSigProc *spsp,
        MMSamplePlayerTickType tt)
{
    switch (tt) {
        case MMSamplePlayerTickType_SUM:
            MMSigProc_setTick(spsp,MMSamplePlayerSigProc_tick_sum);
        case MMSamplePlayerTickType_NOSUM:
            MMSigProc_setTick(spsp,MMSamplePlayerSigProc_tick_no_sum);
    }
}

