#include <stdio.h> 
#include <math.h> 

#include "mm_bus.h"
#include "mm_sample.h"
#include "mm_sampleplayer.h"
#include "mm_sigchain.h"
#include "mm_sigproc.h"
#include "mm_wavtab.h"
#include "mm_sigconst.h"
#include "mm_interp.h" 

/* number of samples to write out */
#define OUTPUT_LENGTH 10000
#define BUS_BLOCK_SIZE OUTPUT_LENGTH
#define BUS_NUM_CHANS 1 

#define WAVTAB_SIZE 8192 
MMSample waveTable[WAVTAB_SIZE];

MMSample output[OUTPUT_LENGTH];

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
    
    int i;
    MMSample x = 0;
    MMSample rate = -100.1;
    while (i < OUTPUT_LENGTH) {
        //int x0 = MM_wrap((int)x - 1, 0, WAVTAB_SIZE);
        //int x1 = MM_wrap((int)x    , 0, WAVTAB_SIZE);
        //int x2 = MM_wrap((int)x + 1, 0, WAVTAB_SIZE);
        //int x3 = MM_wrap((int)x + 2, 0, WAVTAB_SIZE);
        int x0 = (int)x - 1;
        while (x0 < 0) {
            x0 += WAVTAB_SIZE;
        }
        int x1 = (int)x;
        while (x1 < 0) {
            x1 += WAVTAB_SIZE;
        }
        int x2 = (int)x + 1;
        while (x2 >= WAVTAB_SIZE) {
            x2 -= WAVTAB_SIZE;
        }
        int x3 = (int)x + 2;
        while (x3 >= WAVTAB_SIZE) {
            x3 -= WAVTAB_SIZE;
        }
        //output[i] = MM_f_interp_cubic_(x0, waveTable[x0],
        //        x1, waveTable[x1],
        //        x2, waveTable[x2],
        //        x3, waveTable[x3],
        //        x);
        //output[i] = MM_f_interp_cubic_norm_(waveTable[x0],
        //        waveTable[x1],
        //        waveTable[x2],
        //        waveTable[x3],
        //        x);
        output[i] = MM_f_interp_cubic_mu_(waveTable[x0],
                waveTable[x1],
                waveTable[x2],
                waveTable[x3],
                x - (int)x);
        x += rate;
        x = MM_fwrap(x,0,WAVTAB_SIZE);
        i++;
    }


    for (i = 0; i < OUTPUT_LENGTH; i++) {
        printf("%f\n",output[i]);
    }

    return(0);
}

