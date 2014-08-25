#ifndef MM_SIGPROC_H
#define MM_SIGPROC_H 

#include "mm_dllist.h" 

typedef struct __MMSigProc MMSigProc;

typedef enum
{
    MMSigProc_Err_GOOD,
    MMSigProc_Err_UNKNOWN
} MMSigProc_Err;

typedef enum
{
    MMSigProc_State_PLAYING,
    MMSigProc_State_PAUSED,
    MMSigProc_State_DONE,
    MMSigProc_State_UNKNOWN
} MMSigProc_State;

typedef enum
{
    MMSigProc_DoneAction_NONE,
    MMSigProc_DoneAction_FREE
} MMSigProc_DoneAction;

typedef MMSigProc_Err (*MMSigProc_TickFunc)(MMSigProc *);
typedef MMSigProc_Err (*MMSigProc_HandleStateFunc)(MMSigProc *);

struct __MMSigProc {
    MMDLList                     listHead;
    MMSigProc_TickFunc               tick;
/*     MMSigProc_HandleStateFunc handleState; */
    MMSigProc_State                 state;
    MMSigProc_doneAction       doneAction;
};

/* A macro for simplifying the syntax needed to call the tick method of a
 * MMSigProc* or pointers to any instances of its subclasses. */
#define MMSigProc_tick(sp) ((MMSigProc*)sp)->tick((MMSigProc*)sp);

/* #define MMSigProc_handleState(sp) ((MMSigProc*)sp)->handleState((MMSigProc*)sp); */

#endif /* MM_SIGPROC_H */
