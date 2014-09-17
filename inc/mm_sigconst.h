#ifndef MM_SIGCONST_H
#define MM_SIGCONST_H 
/* A signal processor that outputs a constant. Can be set to sum with previous
 * signal or not. */
#include "mm_sigproc.h"
#include "mm_sample.h"
#include "mm_bool.h"
#include "mm_bus.h" 

typedef struct __MMSigConst MMSigConst;

struct __MMSigConst {
    MMSigProc head;
    MMBus outBusHandle;
    MMSample constant;
    MMBool doSum;
};

#define MMSigConst_getOutBusHandle(sc) ((MMSigConst*)sc)->outBusHandle 
#define MMSigConst_setOutBusHandle(sc,val) MMSigConst_getOutBusHandle(sc) = val
#define MMSigConst_getConstant(sc) ((MMSigConst*)sc)->constant
#define MMSigConst_setConstant(sc,val) MMSigConst_getConstant(sc) = val
#define MMSigConst_getDoSum(sc) ((MMSigConst*)sc)->doSum
#define MMSigConst_setDoSum(sc,val) MMSigConst_getDoSum(sc) = val

void MMSigConst_init(MMSigConst *sc);

#endif /* MM_SIGCONST_H */
