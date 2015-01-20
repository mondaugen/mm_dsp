#ifndef MM_ENVELOPE_H
#define MM_ENVELOPE_H 

#include "mm_sample.h" 

typedef enum {
    MMEnvelopeState_OFF,
    MMEnvelopeState_DONE,
    /* more values can be added but must end with the last value */
    MMEnvelopeState_DUMMY /* inheriting classes should start their own 
                             state enums with this state */
} MMEnvelopeState;

typedef struct __MMEnvelope MMEnvelope;

struct __MMEnvelope {
    MMEnvelopeState state;
    MMSample time;
    MMSample (*getCurValue)(MMEnvelope *);
    void     (*incTime)(MMEnvelope *, MMSample); /* Increment time by some delta t */
    void     (*startAttack)(MMEnvelope *);       /* Start the attack portion, could just turn envelope on */
    void     (*startRelease)(MMEnvelope *);      /* Start the release portion, could just turn it off */
    void     (*reset)(MMEnvelope *);             /* Reset envelope */
};

#define MMEnvelope_init(e) ((MMEnvelope*)e)->state = MMEnvelopeState_OFF

/* casts subclasses to MMEnvelope */
#define MMEnvelope_get(e)                   ((MMEnvelope*)e)  
/* set fields */
#define MMEnvelope_setState(e,s)            MMEnvelope_get(e)->state = s
#define MMEnvelope_set_time(e,t)            MMEnvelope_get(e)->time = t 
/* get fields */
#define MMEnvelope_getState(e)              MMEnvelope_get(e)->state 
#define MMEnvelope_get_time(e)              MMEnvelope_get(e)->time
/* Set interfacing methods */
#define MMEnvelope_set_getCurValue(e,gcv)   MMEnvelope_get(e)->getCurValue = gcv
#define MMEnvelope_set_incTime(e,it)        MMEnvelope_get(e)->incTime = it
#define MMEnvelope_set_startAttack(e,sa)    MMEnvelope_get(e)->startAttack = sa
#define MMEnvelope_set_startRelease(e,sr)   MMEnvelope_get(e)->startRelease = sr
#define MMEnvelope_set_reset(e,r)           MMEnvelope_get(e)->reset = r
/* Call interfacing methods */
#define MMEnvelope_getCurValue(e)           MMEnvelope_get(e)->getCurValue(MMEnvelope_get(e))
#define MMEnvelope_incTime(e,dt)            MMEnvelope_get(e)->incTime(MMEnvelope_get(e),dt)
#define MMEnvelope_startAttack(e)           MMEnvelope_get(e)->startAttack(MMEnvelope_get(e))
#define MMEnvelope_startRelease(e)          MMEnvelope_get(e)->startRelease(MMEnvelope_get(e))
#define MMEnvelope_reset(e)                 MMEnvelope_get(e)->reset(MMEnvelope_get(e))

#endif /* MM_ENVELOPE_H */
