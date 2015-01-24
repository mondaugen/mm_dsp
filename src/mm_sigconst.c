#include "mm_sigconst.h"

static void MMSigConst_tick(MMSigProc *sp)
{
    size_t i, j;
    for (i = 0; 
         i < (MMSigConst_getOutBus(sp)->size * MMSigConst_getOutBus(sp)->channels);
         i += MMSigConst_getOutBus(sp)->channels) {
        for (j = 0; j < MMSigConst_getOutBus(sp)->channels; j++) {
            if (MMSigConst_getDoSum(sp)) {
                MMSigConst_getOutBus(sp)->data[i+j] += MMSigConst_getConstant(sp);
            } else {
                MMSigConst_getOutBus(sp)->data[i+j] = MMSigConst_getConstant(sp);
            }
        }
    }
}
        
/* Initializes tick function, bus to NULL, constant to 0 and doSum to 0 */
void MMSigConst_init(MMSigConst *sc, MMBus *outBus, MMSample constant, MMSigConst_doSum doSum)
{
    MMSigProc_init((MMSigProc*)sc);
    MMSigProc_setTick(sc,MMSigConst_tick);
    MMSigConst_setOutBus(sc,outBus);
    MMSigConst_setConstant(sc,constant);
    MMSigConst_setDoSum(sc,doSum);
}

