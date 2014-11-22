#include <stdio.h>
#include <math.h> 
#include <string.h> 

#include "mm_sample.h" 
#include "mm_trapenvedsampleplayer.h" 
#include "mm_sigconst.h" 

#define SAMPLE_RATE 44100
#define WAVTABLE_LENGTH 8192 
#define WAVTABLE_PERIODS 4
#define WAVTABLE_FREQ ((MMSample)SAMPLE_RATE / ((MMSample)WAVTABLE_LENGTH \
        / (MMSample)WAVTABLE_PERIODS))
#define BUS_SIZE 512
#define SILENT_TIME 1 /* 1 second of silence before beginning */ 
#define ATTACK_TIME 5
#define DECAY_TIME 5 
#define SUSTAIN_TIME 5 /* in blocks... */

MMSample waveTable[WAVTABLE_LENGTH];
MMWavTab wt;

void waveTable_init(void)
{
    /* nice major triad */
/*     int ratios[] = {4, 5, 6}; */
    int ratios[] = {4};
    int i, j;
    memset(waveTable,0,WAVTABLE_LENGTH * sizeof(MMSample));
    for (i = 0; i < (sizeof(ratios) / sizeof(int)); i++) {
        for (j = 0; j < WAVTABLE_LENGTH; j++) {
            waveTable[j] += sin(2. * M_PI
                    * (MMSample)j / WAVTABLE_LENGTH * ratios[i]) 
                    / (MMSample)(sizeof(ratios) / sizeof(int));
        }
    }
}

int main (void)
{
    waveTable_init();
    wt.data = waveTable;
    wt.length = WAVTABLE_LENGTH;
    MMTrapEnvedSamplePlayer tesp;
    MMBus *outBus = MMBus_new(BUS_SIZE, 1);
    MMSigChain sigChain;
    MMSigChain_init(&sigChain);
    MMTrapEnvedSamplePlayer_init(&tesp, outBus, BUS_SIZE, 1. / SAMPLE_RATE);
    MMEnvedSamplePlayer_getSamplePlayerSigProc(&tesp).samples = &wt;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(&tesp).loop = 1;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(&tesp).interp =
        MMInterpMethod_CUBIC;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(&tesp).index = 0;
    MMEnvedSamplePlayer_getSamplePlayerSigProc(&tesp).rate =
        pow(2., (53. - 69.) / 12.) * 440.0 / WAVTABLE_FREQ;
    MMSigProc_setState(
            &MMEnvedSamplePlayer_getSamplePlayerSigProc(&tesp),
            MMSigProc_State_PLAYING);
    MMTrapezoidEnv_init(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(&tesp),
            0, 1, ATTACK_TIME, DECAY_TIME);
    /* Add enveloped sampler to signal chain */
    MMSigProc_insertAfter(&sigChain.sigProcs,&tesp);
    /* Add sig const to 0 bus after each iteration */
    MMSigConst sigConst;
    MMSigConst_init(&sigConst, outBus, 0, MMSigConst_doSum_FALSE);
    MMSigProc_insertAfter(&sigChain.sigProcs,&sigConst);
    int k = SAMPLE_RATE * (ATTACK_TIME + SUSTAIN_TIME)
        / BUS_SIZE;
    MMEnvelope_startAttack(&MMTrapEnvedSamplePlayer_getTrapezoidEnv(&tesp));
    int busy = 1;
    while (busy) {
        MMSigProc_tick(&sigChain);
        fwrite(outBus->data, sizeof(MMSample), BUS_SIZE, stdout);
        if (k) {
            k--;
        } else if (k == 0) {
            MMEnvelope_startRelease(
                 &MMTrapEnvedSamplePlayer_getTrapezoidEnv(&tesp));
            k--;
        }
        if (MMTrapEnvedSamplePlayer_getTrapezoidEnv(&tesp).state == MMTrapezoidEnvState_OFF) {
            busy = 0;
        }
    }
    return(0);
}

