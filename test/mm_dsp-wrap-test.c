#include <stdlib.h> 
#include <stdio.h>
#include "mm_wrap.h" 

int main (int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr,"progname low high value\n");
        return(1);
    }
    float x, a, b;
    a = atof(argv[1]);
    b = atof(argv[2]);
    x = atof(argv[3]);
    printf("%f\n", MM_fwrap(x,a,b));
    return(0);
}
