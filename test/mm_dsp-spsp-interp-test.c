#include <stdio.h> 
#include <math.h> 

#include "mm_bus.h"
#include "mm_sample.h"
#include "mm_sampleplayer.h"
#include "mm_sigchain.h"
#include "mm_sigproc.h"
#include "mm_wavtab.h"
#include "mm_sigconst.h"

/* number of samples to write out */
#define OUTPUT_LENGTH 10000
#define BUS_BLOCK_SIZE OUTPUT_LENGTH
#define BUS_NUM_CHANS 1 

#define WAVTAB_SIZE 8192 
MMSample waveTable[WAVTAB_SIZE];

void waveTable_init(void)
{
    int i;
    for (i = 0; i < WAVTAB_SIZE; i++) {
        waveTable[i] = sin((MMSample)i/(MMSample)WAVTAB_SIZE * 2. * M_PI);
    }
}

int main(void)
{
    waveTable_init();

    /* The bus the signal chain is reading/writing */
    MMBus *outBus = MMBus_new(BUS_BLOCK_SIZE,BUS_NUM_CHANS);

    /* a signal chain to put the signal processors into */
    MMSigChain sigChain;
    MMSigChain_init(&sigChain);

    /* A constant that zeros the bus each iteration */
    MMSigConst sigConst;
    MMSigConst_init(&sigConst);
    MMSigConst_setOutBus(&sigConst,outBus);

    /* A sample player */
    MMSamplePlayer samplePlayer;
    MMSamplePlayer_init(&samplePlayer);
    samplePlayer.outBus = outBus;
    /* puts its place holder at the top of the sig chain */
    MMSigProc_insertAfter(&sigChain.sigProcs, &samplePlayer.placeHolder);

    /* put sig constant at the top of the sig chain */
    MMSigProc_insertBefore(&samplePlayer.placeHolder,&sigConst);

    /* Give access to samples of sound as wave table */
    MMWavTab samples;
    samples.data = waveTable;
    samples.length = WAVTAB_SIZE;

    MMSamplePlayerSigProc *spsp;
    spsp = MMSamplePlayerSigProc_new();
    MMSamplePlayerSigProc_init(spsp);
    spsp->samples = &samples;
    spsp->parent = &samplePlayer;
    spsp->loop = 1;
    spsp->interp = MMInterpMethod_CUBIC;
    ((MMSigProc*)spsp)->state = MMSigProc_State_DONE;
    /* insert in signal chain */
    MMSigProc_insertAfter(&samplePlayer.placeHolder, spsp);

    /* start a note playing */
    ((MMSigProc*)spsp)->state = MMSigProc_State_PLAYING;
    spsp->index = 0;
    spsp->rate = 100.1;

    MMSigProc_tick(&sigChain);
    
    int i;
    for (i = 0; i < OUTPUT_LENGTH; i++) {
        printf("%f\n",outBus->data[i]);
    }

    return(0);
}

