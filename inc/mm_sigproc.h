#ifndef MM_SIGPROC_H
#define MM_SIGPROC_H 

#include "mm_dllist.h"

/* An abstract class of signal processor. */

typedef struct __MMSigProc MMSigProc;

typedef enum
{
    MMSigProc_Err_GOOD,
    MMSigProc_Err_UNKNOWN,
    MMSigProc_Err_NOTINIT, /* set when something has not been initialized properly or fully */
    MMSigProc_Err_IDLE, /* Not really an error, output when the sigproc is not do anything (perhaps because it is in "done" state) */
    MMSigProc_Err_JUST_FREED /* Also not an error, just freed the node so we're done */
} MMSigProc_Err;

typedef enum
{
    MMSigProc_State_PLAYING,
    MMSigProc_State_PAUSED,
    MMSigProc_State_DONE,
    MMSigProc_State_UNKNOWN,
    MMSigProc_State_WAIT_FREE, /* Waiting to be freed */
    MMSigProc_State_FREED /* Has been freed */
} MMSigProc_State;

typedef enum
{
    MMSigProc_DoneAction_NONE,
    MMSigProc_DoneAction_FREE
} MMSigProc_DoneAction;

typedef void (*MMSigProc_TickFunc)(MMSigProc *);
typedef MMSigProc_Err (*MMSigProc_HandleStateFunc)(MMSigProc *);
typedef void          (*MMSigProc_FreeFunc)(MMSigProc *);

struct __MMSigProc {
    MMDLList                     listHead;
    MMSigProc_TickFunc               tick;
/*     MMSigProc_HandleStateFunc handleState; */
    MMSigProc_State                 state;
    MMSigProc_DoneAction       doneAction;
};

/* A macro for simplifying the syntax needed to call the tick method of a
 * MMSigProc* or pointers to any instances of its subclasses. */
#define MMSigProc_tick(sp)                  ((MMSigProc*)sp)->tick((MMSigProc*)sp)

#define MMSigProc_setTick(sp,func)          ((MMSigProc*)sp)->tick = func 
#define MMSigProc_getState(sp)              ((MMSigProc*)sp)->state
#define MMSigProc_setState(sp,val)          MMSigProc_getState(sp) = val
#define MMSigProc_getDoneAction(sp)         ((MMSigProc*)sp)->doneAction
#define MMSigProc_setDoneAction(sp,val)     MMSigProc_getDoneAction(sp) = val
#define MMSigProc_insertAfter(whom,who)     MMDLList_insertAfter((MMDLList*)whom,(MMDLList*)who)
#define MMSigProc_insertBefore(whom,who)    MMDLList_insertBefore((MMDLList*)whom,(MMDLList*)who)
#define MMSigProc_addAfterTail(whom,who)    MMDLList_addAfterTail((MMDLList*)whom,(MMDLList*)who)
#define MMSigProc_addBeforeHead(whom,who)   MMDLList_addBeforeHead((MMDLList*)whom,(MMDLList*)who)
#define MMSigProc_remove(who)               MMDLList_remove((MMDLList*)who)


void MMSigProc_defaultTick(MMSigProc *sp);
void MMSigProc_init(MMSigProc *sp);
MMSigProc *MMSigProc_new(void);

/* #define MMSigProc_handleState(sp) ((MMSigProc*)sp)->handleState((MMSigProc*)sp); */

#endif /* MM_SIGPROC_H */
