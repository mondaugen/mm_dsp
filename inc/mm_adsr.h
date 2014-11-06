#ifndef MM_ADSR_H
#define MM_ADSR_H 

#include "mm_sigproc.h" 
#include "mm_sample.h" 
#include "mm_bus.h"


typedef enum {
    MMAdsr_State_DONE,
    MMAdsr_State_ATTACK,
    MMAdsr_State_DECAY,
    MMAdsr_State_SUSTAIN,
    MMAdsr_State_RELEASE,
} MMAdsr_State;

/* ADSR goes from 0 to 1 and then to 0 following a typical ADSR trajectory */
typedef struct __MMAdsr MMAdsr;
struct __MMAdsr {
    MMSigProc head;
    MMBus *bus;              /* bus enveloped by this ADSR */
    MMSample value;         /* the current value of the ADSR */
    MMAdsr_State state;
    MMSample attack;        /* time in seconds */
    MMSample decay;
    MMSample sustain;       /* value between 0 and 1 */
    MMSample release;       /* time in seconds */
    MMSample samplePeriod;  /* the length of one sample period in seconds (1/Fs) */ 
};

void MMAdsr_init(MMAdsr *adsr);

#define MMAdsr_noteOn(adsr,a,d,s,r) do { \
    (adsr)->attack  = a; \
    (adsr)->decay   = d; \
    (adsr)->sustain = s; \
    (adsr)->release = r; \
    (adsr)->state   = MMAdsr_State_ATTACK; \
} while (0)

#define MMAdsr_noteOff(adsr) do { (adsr)->state = MMAdsr_State_RELEASE; } while (0) 

#endif /* MM_ADSR_H */
