#include "mm_busconstmult.h" 

#include <stdint.h> 
#include <assert.h>
#include <stdio.h> 
#include <math.h> 

#include "mm_busconstmult.h" 
#include "test.h" 

#define MM_TEST_ERR_TOL 1.e-6

#define SHORT_TEST_BUS_LEN 8
#define SHORT_TEST_BUS_NCHANS 2 
int busconstmult_short_test(void)
{
    SHOW_TEST_START("busconstmult_short_test");
    MMBusConstMult bcm;
    MMBus *bus;
    bus = MMBus_new(SHORT_TEST_BUS_LEN,SHORT_TEST_BUS_NCHANS);
    MMBusConstMult_init(&bcm, bus, 0.);
    MMSample correct_data[SHORT_TEST_BUS_LEN*SHORT_TEST_BUS_NCHANS];
    size_t i, j;
    for (i = 0; i < (SHORT_TEST_BUS_LEN*SHORT_TEST_BUS_NCHANS); i += bus->channels) {
        correct_data[i] = ((MMSample)i)/(bus->size*bus->channels);
        correct_data[i+1] = -1.*((MMSample)i)/(bus->size*bus->channels);
        bus->data[i] = 1.;
        bus->data[i+1] = -1.;
    }
    bcm.newConst = 1.;
    MMSigProc_tick((MMSigProc*)&bcm);
    for (i = 0; i < (bus->size*bus->channels); i++) {
            assert(FABS(bus->data[i] - correct_data[i]) < MM_TEST_ERR_TOL);
    }
    return 1;
}

int main (void)
{
    int rslt;
    rslt = busconstmult_short_test();
    SHOW_TEST_RESULT(rslt);
    return !rslt;
}
