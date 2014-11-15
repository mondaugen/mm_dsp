#ifndef MM_ENVELOPER_H
#define MM_ENVELOPER_H 

#include <stdint.h> 
#include <string.h> 
#include "mm_sigproc.h"
#include "mm_envelope.h" 
#include "mm_bus.h" 
#include "mm_sample.h" 

typedef struct __MMEnveloper MMEnveloper;

struct __MMEnveloper {
    MMSigProc head;
    MMEnvelope *env;
    MMBus *outBus;
    MMSample tickPeriod;
};

#define MMEnveloper_get(e)          ((MMEnveloper*)e) 
#define MMEnveloper_startAttack(e)  MMEnvelope_startAttack((e)->env) 
#define MMEnveloper_startRelease(e) MMEnvelope_startRelease((e)->env) 
#define MMEnveloper_reset(e)        MMEnvelope_reset((e)->env) 

void MMEnveloper_init(MMEnveloper *enver, MMEnvelope *env, MMBus *outBus, MMSample tickPeriod);

#endif /* MM_ENVELOPER_H */
