#ifndef MM_SIGCHAIN_H
#define MM_SIGCHAIN_H 
#include "mm_sigproc.h" 

typedef struct __MMSigChain MMSigChain;

struct __MMSigChain {
    MMSigProc head; /* sub class of MMSigProc */
    MMSigProc sigProcs; /* The head of the chain of signal processors */
//    size_t trashSize;   /* size of queue that holds SigProcs to be freed */
    /* put queue here ! */
};

void MMSigChain_init(MMSigChain *sc);

#endif /* MM_SIGCHAIN_H */
