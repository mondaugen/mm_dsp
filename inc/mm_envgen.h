#ifndef MM_ENVGEN_H
#define MM_ENVGEN_H 

#include "mm_sigproc.h" 
#include "mm_sample.h"
#include "mm_bus.h" 

typedef struct __MMEnvGen MMEnvGen;

struct __MMEnvGen {
    MMSigProc     head;
    MMBus         *outBus;
    MMSample      deltaTime;
};

/* casts subclasses to MMEnvGen */
#define MMEnvGen_get(e)                   ((MMEnvGen*)e)  
/* set fields */
#define MMEnvGen_setBus(e,b)              MMEnvGen_get(e)->outBus = b
#define MMEnvGen_setDeltaTime(e,dt)       MMEnvGen_get(e)->deltaTime = dt
/* get fields */
#define MMEnvGen_getBus(e)                MMEnvGen_get(e)->outBus
#define MMEnvGen_getDeltaTime(e)          MMEnvGen_get(e)->deltaTime

#define MMEnvGen_init(e,b,dt) MMSigProc_init(e);\
                              MMEnvGen_setBus(e,b);\
                              MMEnvGen_setDeltaTime(e,dt)
                        
#endif /* MM_ENVGEN_H */
