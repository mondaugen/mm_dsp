#include <stdio.h>
#include <string.h> 
#include "mm_enveloper.h" 
#include "mm_trapezoid_env.h" 
#include "mm_sigconst.h" 
#include "mm_sigchain.h" 

#define TICK_PERIOD 0.25 

int main (void)
{
    MMBus *bus;
    MMSigChain sigChain;
    MMTrapezoidEnv env;
    MMSigConst sigConst;
    MMEnveloper enver;
    float time = 0;
    int i, j;

    bus = MMBus_new(10, 2);
    MMSigChain_init(&sigChain);
    MMTrapezoidEnv_init(&env,0,1,2.5,1.6);
    MMSigConst_init(&sigConst,bus,1.,MMSigConst_doSum_FALSE);
    MMEnveloper_init(&enver,(MMEnvelope*)&env,bus,TICK_PERIOD);
    MMSigProc_insertAfter(&sigChain.sigProcs,&sigConst);
    MMSigProc_insertAfter(&sigConst,&enver);

    MMSigProc_tick(&sigChain);
    for (i = 0; i < (bus->size * bus->channels); i += bus->channels) {
        for (j = 0; j < bus->channels; j++) {
            printf("%f ", bus->data[i+j]);
        }
        printf("\n");
    }
    printf("Start attack.\n");
    MMEnveloper_startAttack(&enver);
    MMSigProc_tick(&sigChain);
    for (i = 0; i < (bus->size * bus->channels); i += bus->channels) {
        for (j = 0; j < bus->channels; j++) {
            printf("%f ", bus->data[i+j]);
        }
        printf("\n");
    }
    printf("Sustain.\n");
    MMSigProc_tick(&sigChain);
    for (i = 0; i < (bus->size * bus->channels); i += bus->channels) {
        for (j = 0; j < bus->channels; j++) {
            printf("%f ", bus->data[i+j]);
        }
        printf("\n");
    }
    printf("Start release.\n");
    MMEnveloper_startRelease(&enver);
    MMSigProc_tick(&sigChain);
    for (i = 0; i < (bus->size * bus->channels); i += bus->channels) {
        for (j = 0; j < bus->channels; j++) {
            printf("%f ", bus->data[i+j]);
        }
        printf("\n");
    }
    printf("Aftermath.\n");
    MMEnveloper_reset(&enver);
    MMSigProc_tick(&sigChain);
    for (i = 0; i < (bus->size * bus->channels); i += bus->channels) {
        for (j = 0; j < bus->channels; j++) {
            printf("%f ", bus->data[i+j]);
        }
        printf("\n");
    }
    return 0;
}


