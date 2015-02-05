#include "mm_wavtab_recorder.h"

static void MMWavTabRecorder_tick(MMSigProc *sp)
{
    MMWavTabRecorder *wtr = (MMWavTabRecorder*)sp;
    if (wtr->state == MMWavTabRecorderState_STOPPED) {
        return;
    }
    size_t i;
    /* only records first channel of bus */
    for (i = 0; 
            i < wtr->inputBus->channels * wtr->inputBus->size;
            i += wtr->inputBus->channels) {
        if (wtr->currentIndex >= MMArray_get_length(wtr->buffer)) {
            wtr->state = MMWavTabRecorderState_STOPPED;
            return;
        }
        MMWavTab_get(wtr->buffer, wtr->currentIndex) = wtr->inputBus->data[i];
        (wtr->currentIndex)++;
    }
}

void MMWavTabRecorder_init(MMWavTabRecorder *wtr)
{
    MMSigProc_init((MMSigProc*)wtr);
    MMSigProc_setTick(wtr, MMWavTabRecorder_tick);
    wtr->buffer = NULL;
    wtr->state = MMWavTabRecorderState_STOPPED;
    wtr->inputBus = NULL;
}
