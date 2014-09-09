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
    MMSigProc_Err_IDLE /* Not really an error, output when the sigproc is not do anything (perhaps because it is in "done" state) */
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
typedef void          (*MMSigProc_FreeFunc)(MMSigProc *);

struct __MMSigProc {
    MMDLList                     listHead;
    MMSigChain                      owner; /* The signal chain that this sig proc is a part of. */
    MMSigProc_TickFunc               tick;
/*     MMSigProc_HandleStateFunc handleState; */
    MMSigProc_State                 state;
    MMSigProc_DoneAction       doneAction;
};

/* A macro for simplifying the syntax needed to call the tick method of a
 * MMSigProc* or pointers to any instances of its subclasses. */
#define MMSigProc_tick(sp) ((MMSigProc*)sp)->tick((MMSigProc*)sp);

#define MMSigProc_setTick(sp,func) ((MMSigProc*)sp)->tick = func; 
#define MMSigProc_getState(sp) ((MMSigProc*)sp)->state
#define MMSigProc_setState(sp,val) MMSigProc_getState(sp) = val
#define MMSigProc_getDoneAction(sp) ((MMSigProc*)sp)->doneAction
#define MMSigProc_setDoneAction(sp,val) MMSigProc_getDoneAction(sp) = val
/* Give "from's" owner to "to" */
#define MMSigProc_inheritOwner(from,to) \
    ((MMSigProc*)to)->owner = ((MMSigProc*)from)->owner;
#define MMSigProc_insertAfter(whom,who) do { \
    MMSigProc_inheritOwner(whom,who); \
    MMDLList_insertAfter((MMDLList*)whom,(MMDLList*)who); } while (0);
#define MMSigProc_insertBefore(whom,who) do { \
    MMSigProc_inheritOwner(whom,who); \
    MMDLList_insertBefore((MMDLList*)whom,(MMDLList*)who); } while (0);
#define MMSigProc_addAfterTail(whom,who) do { \
    MMSigProc_inheritOwner(whom,who); \
    MMDLList_addAfterTail((MMDLList*)whom,(MMDLList*)who); } while (0);
#define MMSigProc_addBeforeHead(whom,who) do { \
    MMSigProc_inheritOwner(whom,who); \
    MMDLList_addBeforeHead((MMDLList*)whom,(MMDLList*)who); } while (0);

void MMSigProc_init(MMSigProc *sp);
MMSigProc *MMSigProc_new(void);

/* #define MMSigProc_handleState(sp) ((MMSigProc*)sp)->handleState((MMSigProc*)sp); */

#endif /* MM_SIGPROC_H */
