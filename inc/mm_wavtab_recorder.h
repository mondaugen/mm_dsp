#ifndef MM_WAVTAB_RECORDER_H
#define MM_WAVTAB_RECORDER_H 

#include <stddef.h> 

#include "mm_wavtab.h"
#include "mm_sigproc.h"
#include "mm_bus.h"

typedef struct __MMWavTabRecorder MMWavTabRecorder;

typedef enum {
    MMWavTabRecorderState_STOPPED,
    MMWavTabRecorderState_RECORDING
} MMWavTabRecorderState;

struct __MMWavTabRecorder {
    MMSigProc head;
    MMWavTab *buffer;
    MMWavTabRecorderState state;
    size_t currentIndex;
    /* The absolute maximum length that can be recorded. This
       is because the buffer's length may get redefined
       elsewhere. This ensures that recording only continues
       when valid memory is available and so recordings longer
       than the current buffer length are possible if memory
       is available. */
    size_t maxLength; 
    MMBus *inputBus;
};

void MMWavTabRecorder_init(MMWavTabRecorder * wtr);

#define MMWavTabRecorder_startRecording(wtr) wtr->buffer ? wtr->state = MMWavTabRecorderState_RECORDING : 0

#define MMWavTabRecorder_stopRecording(wtr) wtr->state = MMWavTabRecorderState_STOPPED

#endif /* MM_WAVTAB_RECORDER_H */
