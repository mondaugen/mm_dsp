#ifndef MM_TRAPENVGEN_H
#define MM_TRAPENVGEN_H 

#include "mm_sample.h" 
#include "mm_envgen.h"
#include "mm_trapezoid_env.h" 

typedef struct __MMTrapEnvGen MMTrapEnvGen;

struct __MMTrapEnvGen {
    MMEnvGen             head;
    MMTrapezoidEnv       te;
};

void MMTrapEnvGen_init(  MMTrapEnvGen *teg,
                         MMBus    *bus,
                         MMSample deltaTime,
                         MMSample min,
                         MMSample max,
                         MMSample attackTime,
                         MMSample releaseTime);

#define MMTrapEnvGen_get(te) ((MMTrapEnvGen *)te) 

#endif /* MM_TRAPENVGEN_H */
