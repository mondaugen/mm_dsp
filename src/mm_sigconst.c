#include "mm_sigconst.h"

MMSigProc_Err MMSigConst_tick(MMSigProc *sp)
{
    if (!MMSigConst_getOutBus(sp)) {
        return MMSigProc_Err_NOTINIT; 
    }
    if (MMSigConst_getDoSum(sp)) {
        *(MMSigConst_getOutBus(sp)) += MMSigConst_getConstant(sp);
    } else {
        *(MMSigConst_getOutBus(sp)) = MMSigConst_getConstant(sp);
    }
    return MMSigProc_Err_GOOD;
}
        
/* Initializes tick function, bus to NULL, constant to 0 and doSum to 0 */
void MMSigConst_init(MMSigConst *sc)
{
    MMSigProc_init((MMSigProc*)sc);
    MMSigProc_setTick(sc,MMSigConst_tick);
    MMSigConst_setOutBus(sc,NULL);
    MMSigConst_setConstant(sc,0);
    MMSigConst_setDoSum(sc,0);
}

