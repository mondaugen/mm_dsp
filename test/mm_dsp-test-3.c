/* Testing audio by making a signal processing chain, adding sample players and
 * playing this to stdout (sox or aplay will be able to pipe this to the
 * soundcard). */

#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <sndfile.h>
#include <unistd.h> 
#include <pthread.h>

#include "mm_bus.h"
#include "mm_sample.h"
#include "mm_sampleplayer.h"
#include "mm_sigchain.h"
#include "mm_sigproc.h"
#include "mm_wavtab.h" 
#include "mm_sigconst.h" 

#define MAX_PLAYERS -1

typedef struct __MyTimer MyTimer;

struct __MyTimer {
    size_t currentTime;
    size_t overflow;
    void (*whatToDo)(void*);
    void *data;
};

void MyTimer_tick(MyTimer *t)
{
    t->currentTime += 1;
    if (t->currentTime == t->overflow) {
        t->whatToDo(t->data);
        t->currentTime = 0;
    }
}

struct SampleInfo {
    MMSamplePlayer *samplePlayer;
    MMWavTab *samples;
};

void addPlayer(void *sampleInfo)
{

    static size_t numplayers = 0;
    struct SampleInfo *si = (struct SampleInfo *)sampleInfo;
    MMSamplePlayerSigProc *spsp = MMSamplePlayerSigProc_new();
    MMSamplePlayerSigProc_init(spsp);
    spsp->samples = si->samples;
    spsp->rate = random() / (double) RAND_MAX + 0.1;
    spsp->parent = si->samplePlayer;
    ((MMSigProc*)spsp)->doneAction = MMSigProc_DoneAction_FREE;

    MMDLList_insertAfter((MMDLList*)(&si->samplePlayer->placeHolder),
            (MMDLList*)spsp);
}

int main (int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr,"Usage: %s path-to-soundfile\n",argv[0]);
        exit(-1);
    }

    /* a sample to write all our sound from 1 tick to */
    MMSample sample;
    /* a bus */
    MMBus outBus = &sample;

    /* a signal chain to put our signal processors in. */
    MMSigChain sigChain;
    MMSigChain_init(&sigChain);

    /* a constant that zeros the bus each iteration */
    /* yes, we could just 0 it in the while loop at the bottom, but I wanna try
     * this */
    MMSigConst sigConst;
    MMSigConst_init(&sigConst);
    MMSigConst_setOutBus(&sigConst,outBus);


    /* a sample player */
    MMSamplePlayer samplePlayer;
    MMSamplePlayer_init(&samplePlayer);
    samplePlayer.outBus = outBus;
    /* put its placeHolder at the top of the sigchain */
    MMDLList_insertAfter((MMDLList*)&sigChain.sigProcs,
            (MMDLList*)(&(samplePlayer.placeHolder)));

    /* put the sig constant at the top of the sig chain */
    MMDLList_insertBefore((MMDLList*)&samplePlayer.placeHolder,
            (MMDLList*)&sigConst);

    SF_INFO sfinfo;
    SNDFILE *sndfile;
    memset(&sfinfo,0,sizeof(SF_INFO));
    sndfile = sf_open(argv[1],SFM_READ,&sfinfo);

    if (sfinfo.channels != 1) {
        fprintf(stderr,"Mono files only please.\n");
        exit(-1);
    }

    MMWavTab samples;
    samples.data = (MMSample*)malloc(sizeof(MMSample) * sfinfo.frames *
            sfinfo.channels);

    samples.length = sfinfo.frames * sfinfo.channels;

    if (sf_readf_double(sndfile, samples.data, sfinfo.frames) 
            < sfinfo.frames) {
        fprintf(stderr, "Error reading %s: too few frames.\n",
                argv[1]);
    }

    struct SampleInfo si = { &samplePlayer, &samples };

    MyTimer myTimer = { 0, 2000, addPlayer, &si };

    while (1) {
        MyTimer_tick(&myTimer);
        MMSigProc_tick(&sigChain);
        fwrite(outBus,sizeof(MMSample),1,stdout);
//        *outBus = 0;
    }

    exit(0);
}


