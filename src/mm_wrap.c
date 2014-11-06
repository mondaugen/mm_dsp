#include "mm_wrap.h" 

MMSample MM_fwrap(MMSample x, MMSample a, MMSample b)
{
    if(a>b){
        /* swap entries */
        MMSample tmp = a;
        a = b;
        b = tmp;
    }
    //if(a==b)
    //    return a;
    //if(x >= a){
    //    return fmod(x-a, b-a) + a;
    //}else{
    //    return (b-a) + fmod(x-a, b-a) + a;
    //}
    /* Not the fastest implementation but has the desired result */
    while (x >= b) {
        x -= (b - a);
    }
    while (x < a) {
        x += (b - a);
    }
    return x;
}
