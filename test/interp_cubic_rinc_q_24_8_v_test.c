#include <stdint.h> 
#include <assert.h>
#include <stdio.h> 
#include <math.h> 

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
#define MM_TEST_RINC_ERR_TOL 1.e-5
#define FABS(x) (((x) < 0) ? (-(x)) : (x)) 

static int test_correct_interp(void)
{
    float   in[] = {1.,2.,3.,-3.,-2.,-1.};
    float   out[6];
    int32_t idx0 = 0;
    mm_q8_24_t rate = 1 << 24;
    mm_q8_24_t rinc = 0;
    float   out_correct[] = {1.,2.,3.,-3.,-2.,-1.};
    SHOW_TEST_START("test_correct_interp");
    MM_interp_cubic_rinc_q_8_24_idx_q_24_8_v(out,6,in,6,&idx0,&rate,rinc);
    int n;
    for (n = 0; n < 6; n++) {
        assert(FABS(out[n] - out_correct[n]) < MM_TEST_ERR_TOL);
    }
    return 1;
}

#define TLI_TAB_LEN 256
static int test_loop_interp(void)
{
    float   in[TLI_TAB_LEN];
    float   out[4*TLI_TAB_LEN];
    int32_t idx0 = (TLI_TAB_LEN/2) << 8;
    mm_q8_24_t rate = (int32_t)(-0.5 * (1L << 24));
    mm_q8_24_t rinc = 0;
    float   out_correct[4*TLI_TAB_LEN];
    size_t n;
    for (n = 0; n < TLI_TAB_LEN; n++) {
        in[n] = sinf(((float)n)/TLI_TAB_LEN*2.*M_PI);
    }
    for (n = 0; n < (4*TLI_TAB_LEN); n++) {
        out_correct[n] = sinf((n*0.5)/TLI_TAB_LEN*2.*M_PI);
    }
    SHOW_TEST_START("test_loop_interp");
    MM_interp_cubic_rinc_q_8_24_idx_q_24_8_v(out,4*TLI_TAB_LEN,in,TLI_TAB_LEN,&idx0,&rate,rinc);
    float maxerr = 0.;
    for (n = 0; n < (4*TLI_TAB_LEN); n++) {
        assert(FABS(out[n] - out_correct[n]) < MM_TEST_ERR_TOL);
        maxerr = (maxerr < FABS(out[n] - out_correct[n])) ? FABS(out[n] - out_correct[n]) : maxerr;
    }
    printf("maxerr: %g ", maxerr);
    return 1;
}

/* Test with rate increment */
static int test_loop_interp_rinc(void)
{
    float   in[TLI_TAB_LEN];
    float   out[4*TLI_TAB_LEN];
    int32_t idx0 = 0;
    mm_q8_24_t rate = (mm_q8_24_t)(1 * (1L << 24));
    /* Will be going 5x as fast at the end of the interpolation
     * NOTE: This will not work if the table length is more that 256. */
    mm_q8_24_t rinc = (4*(1L << 24)) / (4*TLI_TAB_LEN);
    float   out_correct[4*TLI_TAB_LEN];
    size_t n;
    for (n = 0; n < TLI_TAB_LEN; n++) {
        in[n] = sinf(((float)n)/TLI_TAB_LEN*2.*M_PI);
    }
    mm_q8_24_t _rate = rate;
    mm_q8_24_t _idx = idx0;
    for (n = 0; n < (4*TLI_TAB_LEN); n++) {
        float tmp = (float)(_idx >> 24);
        tmp += (float)(_idx & 0xffffff) / ((float)(1L << 24));
        out_correct[n] = sinf(tmp/TLI_TAB_LEN*2.*M_PI);
        _idx += _rate;
        _rate += rinc;
    }
    SHOW_TEST_START("test_loop_interp_rinc");
    MM_interp_cubic_rinc_q_8_24_idx_q_24_8_v(out,4*TLI_TAB_LEN,in,TLI_TAB_LEN,&idx0,&rate,rinc);
    assert(rate == _rate);
    float maxerr = 0.;
    for (n = 0; n < (4*TLI_TAB_LEN); n++) {
        assert(FABS(out[n] - out_correct[n]) < MM_TEST_RINC_ERR_TOL);
        maxerr = (maxerr < FABS(out[n] - out_correct[n])) ? FABS(out[n] - out_correct[n]) : maxerr;
    }
    printf("maxerr: %g ", maxerr);
    return 1;
}

int main (void)
{
    int rslt;
    rslt = test_correct_interp();
    SHOW_TEST_RESULT(rslt);
    rslt = rslt && test_loop_interp();
    SHOW_TEST_RESULT(rslt);
    rslt = rslt && test_loop_interp_rinc();
    SHOW_TEST_RESULT(rslt);
    return !rslt;
}
