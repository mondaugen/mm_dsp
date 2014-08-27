/* Testing audio by making a signal processing chain, adding sample players and
 * playing this to stdout (sox or aplay will be able to pipe this to the
 * soundcard). */

#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <sndfile.h>

int main (int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr,"Usage: %s path-to-soundfile\n",argv[0]);
        exit(-1);
    }

    /* a bus */
    MMBus outBus;

    /* a signal chain to put our signal processors in. */
    MMSigChain sigChain;
    MMSigChain_init(&sigChain);

    /* a sample player */
    MMSamplePlayer samplePlayer;
    MMSamplePlayer_init(&samplePlayer);
    samplePlayer.outBus = outBus;
    /* put its placeHolder at the top of the sigchain */
    MMDLList_insertAfter((MMDLList*)(&(sigChain.sigProcs)),
            (MMDLList*)(&(samplePlayer.placeHolder)));

    SFINFO sfinfo;
    SNDFILE *sndfile;
    memset(&sfinfo,0,sizeof(SFINFO));
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
                sfinfo.frames);
    }

    MMSamplePlayerSigProc spsp = MMSamplePlayerSigProc_new();
    MMSamplePlayerSigProc_init(spsp);
    spsp->samples = &samples;
    spsp->rate = 1;
    spsp->parent = &samplePlayer;
    ((MMSigProc*)spsp)->doneAction = MMSigProc_DoneAction_FREE;

    MMDLList_insertAfter((MMDLList*)(&(sigChain.sigProcs)),
            (MMDLList*)spsp);

    while (1) {
        MMSigProc_tick(&sigChain);
        fwrite(outBus,sizeof(MMSample),1,stdout);
    }

    exit(0);
}


