#include "mm_adsr.h" 
#include <stdlib.h>
#include <stdio.h>

#define TS 0.25 /* sample period in seconds */ 

int main (void)
{
    MMAdsr   adsr;
    MMSample sample;
    MMBus    bus = &sample;
    MMSample susTime;
    int      susDone;

    MMAdsr_init(&adsr);
    adsr.samplePeriod = TS;
    adsr.bus = bus;

    /* canonical example, attack and decay have enough time to happen, we
     * sustain for 1 second then decay */
    printf("Canonical ADSR.\n\n");
    MMAdsr_noteOn(&adsr,2,2,0.75,3);
    susTime = 1.;
    susDone = 0;
    while (1) {
        *bus = 1;
        MMSigProc_tick(&adsr);
        printf("%f\n", *bus);
        if (adsr.state == MMAdsr_State_SUSTAIN) {
            susTime -= TS;
        } if ((susTime <= 0) && (!susDone)) {
            MMAdsr_noteOff(&adsr);
            susDone = 1;
        } if (adsr.state == MMAdsr_State_DONE) {
            break;
        }
    }

    printf("No attack ADSR.\n\n");
    MMAdsr_noteOn(&adsr,0,2,0.75,3);
    susTime = 1.;
    susDone = 0;
    while (1) {
        *bus = 1;
        MMSigProc_tick(&adsr);
        printf("%f\n", *bus);
        if (adsr.state == MMAdsr_State_SUSTAIN) {
            susTime -= TS;
        } if ((susTime <= 0) && (!susDone)) {
            MMAdsr_noteOff(&adsr);
            susDone = 1;
        } if (adsr.state == MMAdsr_State_DONE) {
            break;
        }
    }

    printf("No decay ADSR.\n\n");
    MMAdsr_noteOn(&adsr,3,0,0.75,3);
    susTime = 1.;
    susDone = 0;
    while (1) {
        *bus = 1;
        MMSigProc_tick(&adsr);
        printf("%f\n", *bus);
        if (adsr.state == MMAdsr_State_SUSTAIN) {
            susTime -= TS;
        } if ((susTime <= 0) && (!susDone)) {
            MMAdsr_noteOff(&adsr);
            susDone = 1;
        } if (adsr.state == MMAdsr_State_DONE) {
            break;
        }
    }

    printf("No release ADSR.\n\n");
    MMAdsr_noteOn(&adsr,3,2,0.75,0);
    susTime = 1.;
    susDone = 0;
    while (1) {
        *bus = 1;
        MMSigProc_tick(&adsr);
        printf("%f\n", *bus);
        if (adsr.state == MMAdsr_State_SUSTAIN) {
            susTime -= TS;
        } if ((susTime <= 0) && (!susDone)) {
            MMAdsr_noteOff(&adsr);
            susDone = 1;
        } if (adsr.state == MMAdsr_State_DONE) {
            break;
        }
    }

    printf("Interrupt attack ADSR.\n\n");
    MMAdsr_noteOn(&adsr,2,2,0.75,1);
    susTime = 1.;
    susDone = 0;
    while (1) {
        *bus = 1;
        MMSigProc_tick(&adsr);
        printf("%f\n", *bus);
        if ((adsr.attack < 0.5) && (adsr.state == MMAdsr_State_ATTACK)) {
            MMAdsr_noteOff(&adsr);
        }
        if (adsr.state == MMAdsr_State_DONE) {
            break;
        }
    }

    printf("Interrupt attack, no release ADSR.\n\n");
    MMAdsr_noteOn(&adsr,2,2,0.75,0);
    susTime = 1.;
    susDone = 0;
    while (1) {
        *bus = 1;
        MMSigProc_tick(&adsr);
        printf("%f\n", *bus);
        if ((adsr.attack < 0.5) && (adsr.state == MMAdsr_State_ATTACK)) {
            MMAdsr_noteOff(&adsr);
        }
        if (adsr.state == MMAdsr_State_DONE) {
            break;
        }
    }


    return 0;
}
