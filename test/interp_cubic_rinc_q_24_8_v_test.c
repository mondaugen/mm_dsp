#include <assert.h>
#include <stdio.h> 

#include "mm_interp.h"

#define SHOW_TEST_START(msg) \
   printf("Doing test: %s... ",msg) 
#define SHOW_TEST_RESULT(res) \
    if (res) { \
        printf("passed\n"); \
    } else { \
        printf("failed\n"); \
    }

#define MM_TEST_ERR_TOL 1.e-6
#define FABS(x) ((x < 0) ? (-x) : (x)) 

static int test_correct_interp(void)
{
    float   in[] = {1.,2.,3.,-3.,-2.,-1.};
    float   out[6];
    int32_t idx0 = 0;
    int32_t rate = 1 << 8;
    int32_t rinc = 0;
    float   out_correct[] = {1.,2.,3.,-3.,-2.,-1.};
    SHOW_TEST_START("test_correct_interp");
    MM_interp_cubic_rinc_q_24_8_v(out,6,in,6,&idx0,&rate,rinc);
    int n;
    for (n = 0; n < 6; n++) {
        assert(FABS(out[n] - out_correct[n]) < MM_TEST_ERR_TOL);
    }
    return 1;
}

static int test_loop_interp(void)
{
    float   in[] = {1.,2.,3.,-3.,-2.,-1.};
    float   out[15];
    int32_t idx0 = 3;
    int32_t rate = (int32_t)(0.5 * 0x100);
    int32_t rinc = 0;
    // float   out_correct[] = {-3., 0, 3., 2.5, 2.,
    //                          1.5, 1., .5, 0., -.5,
    //                          -1., -1.5, -2., -2.5, -3.};
    float   out_correct[] = {-3., 0, 3., 2.5, 2.,
                             1.5, 1., .5, 0., -.5,
                             -1., -1.5, -2., -2.5, -3.};
    SHOW_TEST_START("test_loop_interp");
    MM_interp_cubic_rinc_q_24_8_v(out,15,in,6,&idx0,&rate,rinc);
    int n;
    for (n = 0; n < 15; n++) {
        assert(FABS(out[n] - out_correct[n]) < MM_TEST_ERR_TOL);
    }
    return 1;
}

int main (void)
{
    int rslt;
    rslt = test_correct_interp();
    SHOW_TEST_RESULT(rslt);
    rslt = rslt && test_loop_interp();
    SHOW_TEST_RESULT(rslt);
    return !rslt;
}
