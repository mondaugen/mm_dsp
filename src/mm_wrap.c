#include "mm_wrap.h" 

int MM_wrap(int x, int a, int b)
{
    if(a>b){
        /* swap entries */
        int tmp = a;
        a = b;
        b = tmp;
    }
    int N = 0;
    if (x < a) {
        N = (int) ((x - a) / (b - a)) - 1;
    }
    x -= N * (b - a);
    N = 0;
    if (x >= b) {
        N = (int) ((x - b) / (b - a)) + 1;
    }
    x -= N * (b - a);
    return x;
}

int64_t MM_wrapl(int64_t x, int64_t a, int64_t b)
{
    if(a>b){
        /* swap entries */
        int64_t tmp = a;
        a = b;
        b = tmp;
    }
    int64_t N = 0;
    if (x < a) {
        N = (int64_t) ((x - a) / (b - a)) - 1;
    }
    x -= N * (b - a);
    N = 0;
    if (x >= b) {
        N = (int64_t) ((x - b) / (b - a)) + 1;
    }
    x -= N * (b - a);
    return x;
}

MMSample MM_fwrap(MMSample x, MMSample a, MMSample b)
{
    if(a>b){
        /* swap entries */
        MMSample tmp = a;
        a = b;
        b = tmp;
    }
    int N = 0;
    if (x < a) {
        N = (int) ((x - a) / (b - a)) - 1;
    }
    x -= N * (b - a);
    N = 0;
    if (x >= b) {
        N = (int) ((x - b) / (b - a)) + 1;
    }
    x -= N * (b - a);
    return x;
}
