#include "mm_sampleplayer.h" 
#include "mm_stdlib.h" 
#include <string.h> 

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
                MMSamplePlayerSigProc_getSampleInterpCubicMu_sum_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            default:
                *(spsp->parent->outBus->data + i) = 0;
                break;
        }
        spsp->index += spsp->rate;
        if (spsp->loop) {
            spsp->index = MM_fwrap(spsp->index, 0,
                    MMArray_get_length(spsp->samples));
        } else if ((spsp->index >= MMArray_get_length(spsp->samples))
                || (spsp->index < 0)) {
            MMSigProc_setState(spsp,MMSigProc_State_DONE);
            i = (spsp->parent->outBus->size * spsp->parent->outBus->channels);
        }
    }
    /* we handle the doneAction */
    if ((MMSigProc_getState(spsp) == MMSigProc_State_DONE) 
            && (MMSigProc_getDoneAction(spsp) == MMSigProc_DoneAction_FREE)) {
        sp->state = MMSigProc_State_WAIT_FREE;
    }
}

/* Writes a sample according to lookup speed and wavetable into the busses of
 * the sample player. No interpolation for now. */
static void MMSamplePlayerSigProc_tick_no_sum(MMSigProc *sp)
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
                MMSamplePlayerSigProc_getSampleInterpNone_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            case MMInterpMethod_LINEAR:
                MMSamplePlayerSigProc_getSampleInterpLinear_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            case MMInterpMethod_CUBIC:
                MMSamplePlayerSigProc_getSampleInterpCubicMu_(spsp,
                        spsp->parent->outBus->data + i);
                break;
            default:
                *(spsp->parent->outBus->data + i) = 0;
                break;
        }
        spsp->index += spsp->rate;
        if (spsp->loop) {
            spsp->index = MM_fwrap(spsp->index, 0,
                    MMArray_get_length(spsp->samples));
        } else if ((spsp->index >= MMArray_get_length(spsp->samples))
                || (spsp->index < 0)) {
            MMSigProc_setState(spsp,MMSigProc_State_DONE);
            i = (spsp->parent->outBus->size * spsp->parent->outBus->channels);
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

void MMSamplePlayerSigProc_init(MMSamplePlayerSigProc *spsp, MMSamplePlayerTickType tt)
{
    memset(spsp,0,sizeof(MMSamplePlayerSigProc));
    MMSigProc_init((MMSigProc*)spsp);
    if (tt == MMSamplePlayerTickType_NOSUM) {
        MMSigProc_setTick(spsp,MMSamplePlayerSigProc_tick_no_sum);
    } else {
        MMSigProc_setTick(spsp,MMSamplePlayerSigProc_tick_sum);
    }
}

MMSamplePlayer *MMSamplePlayer_new(void)
{
    return (MMSamplePlayer*)malloc(sizeof(MMSamplePlayer));
}

void MMSamplePlayer_init(MMSamplePlayer *sp)
{
    memset(sp,0,sizeof(MMSamplePlayer));
    MMSigProc_init(&sp->placeHolder);
}
